/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppMotionSyncMicrophoneModel.hpp"
#include "CubismModelMotionSyncSettingJson.hpp"
#include "LAppDefine.hpp"
#include "LAppMicrophoneDelegate.hpp"
#include "LAppMotionSyncDefine.hpp"
#include "LAppPal.hpp"
#include "LAppTextureManager.hpp"
#include "Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp"

using namespace Csm;
using namespace MotionSync;
using namespace LAppDefine;
using namespace LAppMotionSyncDefine;

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
        if (DebugLogEnable)
        {
            LAppPal::PrintLogLn("[APP]delete buffer: %s", path);
        }
        LAppPal::ReleaseBytes(buffer);
    }
}

LAppMotionSyncMicrophoneModel::LAppMotionSyncMicrophoneModel(csmString executeAbsolutePath)
    : CubismUserModel()
    , _executeAbsolutePath(executeAbsolutePath)
    , _modelSetting(NULL)
    , _motionSync(NULL)
{
}

LAppMotionSyncMicrophoneModel::~LAppMotionSyncMicrophoneModel()
{
    if (_modelSetting)
    {
        delete _modelSetting;
    }

    if (_motionSync)
    {
        _soundData.Release();
        CubismMotionSync::Delete(_motionSync);
    }
}

void LAppMotionSyncMicrophoneModel::LoadAssets(const csmString fileName)
{
    _modelHomeDir = _executeAbsolutePath + csmString(ResourcesPath) + fileName + "/";
    
    if (_debugMode)
    {
        LAppPal::PrintLogLn("[APP]load model setting: %s", fileName.GetRawString());
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

    CreateRenderer();

    // テクスチャのセットアップ
    SetupTextures();
}

void LAppMotionSyncMicrophoneModel::Update()
{
    int width, height;
    // ウィンドウサイズを取得
    glfwGetWindowSize(LAppMicrophoneDelegate::GetInstance()->GetWindow(), &width, &height);

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

    // モデルのパラメータを更新
    UpdateModelParam();

    // モデルの描画を更新
    Draw(projection); ///< 参照渡しなのでprojectionは変質する
}

void LAppMotionSyncMicrophoneModel::SetupModel()
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

    //Layout
    csmMap<csmString, csmFloat32> layout;
    _modelSetting->GetLayoutMap(layout);
    _modelMatrix->SetupFromLayout(layout);

    // パラメータを保存
    _model->SaveParameters();

    _updating = false;
    _initialized = true;

    // MotionSync
    const csmChar* fileName = _modelSetting->GetMotionSyncJsonFileName();

    if (strcmp(fileName, ""))
    {
        if (_debugMode)
        {
            LAppPal::PrintLogLn("[APP]load motionSync setting: %s", fileName);
        }

        const csmString path = csmString(_modelHomeDir) + fileName;
        buffer = CreateBuffer(path.GetRawString(), &size);

        _motionSync = CubismMotionSync::Create(_model, buffer, size, SamplesPerSec, _executeAbsolutePath);

        if (!_motionSync)
        {
            LAppPal::PrintLogLn("Failed to SetupModel().");
            return;
        }

        DeleteBuffer(buffer, path.GetRawString());

        // 音声データ
        _soundData.SetupMicrophone(Channels, SamplesPerSec, BitDepth, 0);
        _motionSync->SetSoundBuffer(0, _soundData.GetBuffer());
    }
}

void LAppMotionSyncMicrophoneModel::SetupTextures()
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

        LAppTextureManager::TextureInfo* texture = LAppMicrophoneDelegate::GetInstance()->GetTextureManager()->CreateTextureFromPngFile(texturePath.GetRawString());
        const csmInt32 glTextueNumber = texture->id;

        // OpenGL
        GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->BindTexture(modelTextureNumber, glTextueNumber);
    }

    // 乗算済みアルファ値の有効化・無効化を設定
    GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->IsPremultipliedAlpha(false);
}

void LAppMotionSyncMicrophoneModel::UpdateModelParam()
{
        const csmFloat32 deltaTimeSeconds = LAppPal::GetDeltaTime();
        _userTimeSeconds += deltaTimeSeconds;
    
        // 不透明度
        _opacity = _model->GetModelOpacity();
    
        // モーションシンクプラグイン
        if (_motionSync != NULL)
        {
            _motionSync->UpdateParameters(_model, deltaTimeSeconds);
        }
    
        _model->Update();
}

void LAppMotionSyncMicrophoneModel::Draw(Csm::CubismMatrix44& matrix)
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
