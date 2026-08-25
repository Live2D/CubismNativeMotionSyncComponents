/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppMotionSyncLinkModel.hpp"
#include "Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp"
#include "Utils/CubismString.hpp"
#include "LAppDefine.hpp"
#include "LAppMotionSyncDefine.hpp"
#include "LAppPal.hpp"
#include "LAppTextureManager.hpp"
#include "LAppMotionEventDefine.hpp"

using namespace Csm;
using namespace MotionSync;
using namespace LAppMotionSyncDefine;
using namespace LAppCubismMotionEventDefine;

namespace {
    csmByte* CreateBuffer(const csmChar* path, csmSizeInt* size)
    {
        if (LAppDefine::DebugLogEnable)
        {
            LAppPal::PrintLogLn("[APP]create buffer: %s ", path);
        }
        return LAppPal::LoadFileAsBytes(path, size);
    }

    void DeleteBuffer(csmByte* buffer, const csmChar* path = "")
    {
        if (LAppDefine::DebugLogEnable)
        {
            LAppPal::PrintLogLn("[APP]delete buffer: %s", path);
        }
        LAppPal::ReleaseBytes(buffer);
    }
}

LAppMotionSyncLinkModel::LAppMotionSyncLinkModel()
    : _modelSetting(NULL)
    , _userTimeSeconds(0)
    , _motionSync(NULL)
    , _motionIndex(-1)
    , _isMotionSync(NULL)
    , _currentMotionSyncSettingIndex(0)
    , _isEnableMotionSync(false)
    , _currentMotionSyncMotion(NULL)
{}

LAppMotionSyncLinkModel::~LAppMotionSyncLinkModel()
{
    ReleaseMotions();

    if (_motionSync)
    {
        _soundData.Release();
        CubismMotionSync::Delete(_motionSync);
    }

    delete(_modelSetting);
}

void LAppMotionSyncLinkModel::LoadAssets(const Live2D::Cubism::Framework::csmString fileName)
{
    _modelHomeDir = csmString(LAppDefine::ResourcesPath) + fileName + "/";

    if (_debugMode)
    {
        LAppPal::PrintLogLn("[APP]load model setting: %s", fileName);
    }

    csmSizeInt size;
    const csmString path = _modelHomeDir + fileName + ".model3.json";

    csmByte* buffer = CreateBuffer(path.GetRawString(), &size);
    _modelSetting = new CubismModelMotionSyncSettingJson(buffer, size);
    DeleteBuffer(buffer, path.GetRawString());

    SetupModel();

    if (_model == NULL)
    {
        LAppPal::PrintLogLn("Failed to LoadAssets().");
        return;
    }

    // レンダラーの作成
    CreateRenderer();

    // テクスチャのセットアップ
    SetupTextures();
}

void LAppMotionSyncLinkModel::Update()
{
    const csmFloat32 deltaTimeSeconds = LAppPal::GetDeltaTime();
    _userTimeSeconds += deltaTimeSeconds;

    // モーションによるパラメータ更新の有無
    csmBool motionUpdated = false;

    //-----------------------------------------------------------------
    _model->LoadParameters(); // 前回セーブされた状態をロード
    motionUpdated = _motionManager->UpdateMotion(_model, deltaTimeSeconds); // モーションを更新
    _model->SaveParameters(); // 状態を保存
    //-----------------------------------------------------------------

    // 音声データ更新
    // NOTE: バッファが破棄されるので、モーションが再生されている間のみ更新する。
    if (_isMotionSync && _isEnableMotionSync)
    {
        _soundData.Update();
    }

    // 不透明度
    _opacity = _model->GetModelOpacity();

    // 物理演算の設定
    if (_physics != NULL)
    {
        _physics->Evaluate(_model, deltaTimeSeconds);
    }

    // ポーズの設定
    if (_pose != NULL)
    {
        _pose->UpdateParameters(_model, deltaTimeSeconds);
    }

    if (_motionSync != NULL)
    {
        _motionSync->UpdateParameters(_model, deltaTimeSeconds);
    }

    _model->Update();

    int width, height;
    // ウィンドウサイズを取得
    glfwGetWindowSize(LAppMotionSyncLinkDelegate::GetInstance()->GetWindow(), &width, &height);

    Csm::CubismMatrix44 projection;
    // 念のため単位行列に初期化
    projection.LoadIdentity();

    if (_model->GetCanvasWidth() > 1.0f && width < height)
    {
        // 横に長いモデルを縦長ウィンドウに表示する際モデルの横サイズでscaleを算出する
        GetModelMatrix()->SetWidth(2.0f);
        projection.Scale(1.0f, static_cast<float>(width) / static_cast<float>(height));
    }
    else
    {
        projection.Scale(static_cast<float>(height) / static_cast<float>(width), 1.0f);
    }

    // モデルの描画を更新
    Draw(projection); ///< 参照渡しなのでprojectionは変質する
}

void LAppMotionSyncLinkModel::ChangeNextMotion()
{
    _motionIndex += 1;
    if (_motionIndex < 0 || _motionIndex >= _motions.GetSize())
    {
        _motionIndex = 0;
    }

    StartMotion("", _motionIndex, LAppDefine::PriorityForce, LAppCubismMotionEventDefine::OnFinishedMotion, NULL);
}

Csm::CubismMotionQueueEntryHandle LAppMotionSyncLinkModel::StartMotion(const Csm::csmChar* group, Csm::csmInt32 no,
    Csm::csmInt32 priority, Csm::ACubismMotion::FinishedMotionCallback onFinishedMotionHandler,
    Csm::ACubismMotion::BeganMotionCallback onBeganMotionHandler)
{
    if (priority == LAppDefine::PriorityForce)
    {
        _motionManager->SetReservePriority(priority);
    }
    else if (!_motionManager->ReserveMotion(priority))
    {
        if (_debugMode)
        {
            LAppPal::PrintLogLn("[APP]can't start motion.");
        }
        return InvalidMotionQueueEntryHandleValue;
    }

    const csmString fileName = _modelSetting->GetMotionFileName(group, no);

    //ex) idle_0
    csmString name = Utils::CubismString::GetFormatedString("%s_%d", group, no);
    CubismMotion* motion = static_cast<CubismMotion*>(_motions[name.GetRawString()]);
    csmBool autoDelete = false;

    if (motion == NULL)
    {
        csmString path = fileName;
        path = _modelHomeDir + path;

        csmByte* buffer;
        csmSizeInt size;
        buffer = CreateBuffer(path.GetRawString(), &size);
        motion = static_cast<CubismMotion*>(LoadMotion(buffer, size, NULL, onFinishedMotionHandler, onBeganMotionHandler));

        if (motion)
        {
            csmFloat32 fadeTime = _modelSetting->GetMotionFadeInTimeValue(group, no);
            if (fadeTime >= 0.0f)
            {
                motion->SetFadeInTime(fadeTime);
            }

            fadeTime = _modelSetting->GetMotionFadeOutTimeValue(group, no);
            if (fadeTime >= 0.0f)
            {
                motion->SetFadeOutTime(fadeTime);
            }
            autoDelete = true; // 終了時にメモリから削除
        }

        DeleteBuffer(buffer, path.GetRawString());
    }
    else
    {
        motion->SetBeganMotionHandler(onBeganMotionHandler);
        motion->SetFinishedMotionHandler(onFinishedMotionHandler);
    }

    if (!_motionSyncSettingMap.IsExist(fileName) || !_soundFileMap.IsExist(fileName))
    {
        LAppPal::PrintLogLn("[APP]No exist: %s", fileName.GetRawString());
        return InvalidMotionQueueEntryHandleValue;
    }

    // MotionSync
    csmString useMotionSyncSettingName = _motionSyncSettingMap[fileName];
    csmString useSoundFileName = _soundFileMap[fileName];

    csmInt32 motionSyncSettingIndex = -1;
    for (csmInt32 i = 0; i < _motionSync->GetCubismMotionSyncData()->GetSettingListSize(); i++)
    {
        csmString settingIdName = _motionSync->GetCubismMotionSyncData()->GetMeta().dictionary.At(i).name;

        if (settingIdName == useMotionSyncSettingName)
        {
            motionSyncSettingIndex = i;
            break;
        }
    }

    if (motionSyncSettingIndex != -1)
    {
        _currentSoundFileName = useSoundFileName;
        _currentMotionSyncSettingIndex = motionSyncSettingIndex;
        // このモーションが現在のモーションシンクの所有者
        _currentMotionSyncMotion = motion;

        // 音声ファイルのロードと再生の開始
        StartMotionSync();
    }
    else
    {
        if (_debugMode)
        {
            LAppPal::PrintLogLn("[APP]Failed set motionSync: [%s_%d]", group, no);
        }

        _currentSoundFileName = "";
        _currentMotionSyncSettingIndex = -1;
    }

    if (_debugMode)
    {
        LAppPal::PrintLogLn("[APP]start motion: [%s_%d]", group, no);
    }
    return  _motionManager->StartMotionPriority(motion, autoDelete, priority);
}

void LAppMotionSyncLinkModel::OnFinishedMotion(Csm::ACubismMotion* self)
{
    // 早送りなどでモーションを切り替えた場合、旧モーションの終了コールバックが
    // 新しく開始したモーションシンクを止めてしまうのを防ぐ。
    // 現在のモーションシンクを再生しているモーションが終了した場合のみ破棄する。
    if (self != _currentMotionSyncMotion)
    {
        return;
    }

    // 音声再生の終了と音声バッファのクリア
    if (_motionSync && _currentMotionSyncSettingIndex >= 0)
    {
        _motionSync->SetSoundBuffer(_currentMotionSyncSettingIndex, NULL);
    }
    _soundData.Release();

    // モーションシンクの無効化
    _isEnableMotionSync = false;
    _currentMotionSyncMotion = NULL;

    if (_debugMode)
    {
        LAppPal::PrintLogLn("[APP] Finish motion sync: %s%s", _modelHomeDir.GetRawString(), _currentSoundFileName.GetRawString());
    }
}

void LAppMotionSyncLinkModel::StartMotionSync()
{
    if (_currentMotionSyncSettingIndex < 0 || _currentSoundFileName.GetLength() < 1)
    {
        if (_debugMode)
        {
            LAppPal::PrintLogLn("[APP]Failed begin motion sync.\n_currentMotionSyncSettingIndex: %d, _currentSoundFileName length: %d",
                _currentMotionSyncSettingIndex, _currentSoundFileName.GetLength());
        }
        return;
    }

    // モーションシンクの有効化
    _isEnableMotionSync = true;

    // 音声再生の開始
    if (_debugMode)
    {
        LAppPal::PrintLogLn("[APP] Begin motion sync: %s%s", _modelHomeDir.GetRawString(), _currentSoundFileName.GetRawString());
    }
    _soundData.LoadFile(_modelHomeDir + _currentSoundFileName, 0);
    _motionSync->SetSoundBuffer(_currentMotionSyncSettingIndex, _soundData.GetBuffer());
}

void LAppMotionSyncLinkModel::SetupModel()
{
    _updating = true;
    _initialized = false;

    csmByte* buffer;
    csmSizeInt size;

    //Cubism Model
    if (strcmp(_modelSetting->GetModelFileName(), "") != 0)
    {
        csmString path = _modelSetting->GetModelFileName();
        path = _modelHomeDir + path;

        if (_debugMode)
        {
            LAppPal::PrintLogLn("[APP]create model: %s", _modelSetting->GetModelFileName());
        }

        buffer = CreateBuffer(path.GetRawString(), &size);
        LoadModel(buffer, size, _mocConsistency);
        DeleteBuffer(buffer, path.GetRawString());
    }

    if (_modelSetting == NULL || _modelMatrix == NULL)
    {
        LAppPal::PrintLogLn("Failed to SetupModel().");
        return;
    }

    //Physics
    if (strcmp(_modelSetting->GetPhysicsFileName(), "") != 0)
    {
        csmString path = _modelSetting->GetPhysicsFileName();
        path = _modelHomeDir + path;

        buffer = CreateBuffer(path.GetRawString(), &size);
        LoadPhysics(buffer, size);
        DeleteBuffer(buffer, path.GetRawString());
    }

    //Pose
    if (strcmp(_modelSetting->GetPoseFileName(), "") != 0)
    {
        csmString path = _modelSetting->GetPoseFileName();
        path = _modelHomeDir + path;

        buffer = CreateBuffer(path.GetRawString(), &size);
        LoadPose(buffer, size);
        DeleteBuffer(buffer, path.GetRawString());
    }

    //Layout
    csmMap<csmString, csmFloat32> layout;
    _modelSetting->GetLayoutMap(layout);
    _modelMatrix->SetupFromLayout(layout);

    // パラメータを保存
    _model->SaveParameters();

    // Motion
    for (csmInt32 i = 0; i < _modelSetting->GetMotionGroupCount(); i++)
    {
        const csmChar* group = _modelSetting->GetMotionGroupName(i);
        PreloadMotionGroup(group);
    }

    _motionManager->StopAllMotions();

    // MotionSync
    const csmChar* fileName = _modelSetting->GetMotionSyncJsonFileName();

    if (std::strcmp(fileName, "") != 0)
    {
        if (_debugMode)
        {
            LAppPal::PrintLogLn("[APP]load motionSync setting: %s", fileName);
        }

        const csmString path = csmString(_modelHomeDir) + fileName;
        buffer = CreateBuffer(path.GetRawString(), &size);

        _motionSync = CubismMotionSync::Create(_model, buffer, size, SamplesPerSec);

        if (!_motionSync)
        {
            LAppPal::PrintLogLn("Failed to SetupModel().");
            return;
        }

        DeleteBuffer(buffer, path.GetRawString());

        // モーション名とそれに紐づくデータを保存
        _modelSetting->GetMotionSyncLinkSettingMap(_motionSyncSettingMap);
        _modelSetting->GetMotionSyncLinkAudioMap(_soundFileMap);
        _motionIndex = -1;
        _isMotionSync = true;
    }

    _updating = false;
    _initialized = true;
}

void LAppMotionSyncLinkModel::SetupTextures()
{
    for (csmInt32 modelTextureNumber = 0; modelTextureNumber < _modelSetting->GetTextureCount(); modelTextureNumber++)
    {
        // テクスチャ名が空文字だった場合はロード・バインド処理をスキップ
        if (!strcmp(_modelSetting->GetTextureFileName(modelTextureNumber), ""))
        {
            continue;
        }

        // OpenGLのテクスチャユニットにテクスチャをロードする
        csmString texturePath = _modelSetting->GetTextureFileName(modelTextureNumber);
        texturePath = _modelHomeDir + texturePath;

        LAppTextureManager::TextureInfo* texture = LAppMotionSyncLinkDelegate::GetInstance()->GetTextureManager()->CreateTextureFromPngFile(texturePath.GetRawString());
        const csmInt32 glTextueNumber = texture->id;

        // OpenGL
        GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->BindTexture(modelTextureNumber, glTextueNumber);
    }

    // 乗算済みアルファ値の有効化・無効化を設定
    GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->IsPremultipliedAlpha(false);
}

void LAppMotionSyncLinkModel::PreloadMotionGroup(const Csm::csmChar* group)
{
    const csmInt32 count = _modelSetting->GetMotionCount(group);

    for (csmInt32 i = 0; i < count; i++)
    {
        //ex) idle_0
        csmString name = Utils::CubismString::GetFormatedString("%s_%d", group, i);
        csmString path = _modelSetting->GetMotionFileName(group, i);
        path = _modelHomeDir + path;

        if (_debugMode)
        {
            LAppPal::PrintLogLn("[APP]load motion: %s => [%s_%d] ", path.GetRawString(), group, i);
        }

        csmByte* buffer;
        csmSizeInt size;
        buffer = CreateBuffer(path.GetRawString(), &size);
        CubismMotion* tmpMotion = static_cast<CubismMotion*>(LoadMotion(buffer, size, name.GetRawString()));

        if (tmpMotion)
        {
            csmFloat32 fadeTime = _modelSetting->GetMotionFadeInTimeValue(group, i);
            if (fadeTime >= 0.0f)
            {
                tmpMotion->SetFadeInTime(fadeTime);
            }

            fadeTime = _modelSetting->GetMotionFadeOutTimeValue(group, i);
            if (fadeTime >= 0.0f)
            {
                tmpMotion->SetFadeOutTime(fadeTime);
            }

            if (_motions[name] != NULL)
            {
                ACubismMotion::Delete(_motions[name]);
            }
            _motions[name] = tmpMotion;
        }

        DeleteBuffer(buffer, path.GetRawString());
    }
}

void LAppMotionSyncLinkModel::ReleaseMotions()
{
    for (csmMap<csmString, ACubismMotion*>::const_iterator iter = _motions.Begin(); iter != _motions.End(); ++iter)
    {
        ACubismMotion::Delete(iter->Second);
    }

    _motions.Clear();
}

void LAppMotionSyncLinkModel::Draw(Csm::CubismMatrix44& matrix)
{
    if (!_model)
    {
        return;
    }

    // 現在の行列に行列を乗算
    matrix.MultiplyByMatrix(_modelMatrix);

    // 行列をモデルビュープロジェクション行列を設定
    GetRenderer<Csm::Rendering::CubismRenderer_OpenGLES2>()->SetMvpMatrix(&matrix);

    // モデルの描画を命令・実行する
    GetRenderer<Csm::Rendering::CubismRenderer_OpenGLES2>()->DrawModel();
}
