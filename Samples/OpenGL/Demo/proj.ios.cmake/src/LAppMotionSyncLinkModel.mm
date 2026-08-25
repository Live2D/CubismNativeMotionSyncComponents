/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import "LAppMotionSyncLinkModel.h"
#import "CubismModelMotionSyncSettingJson.hpp"
#import "LAppDefine.h"
#import "AppMotionSyncLinkDelegate.h"
#import "LAppMotionSyncDefine.h"
#import "LAppMotionEventDefine.h"
#import "LAppPal.h"
#import "LAppTextureManager.h"
#import "Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp"
#import <Utils/CubismString.hpp>
#import <cstring>

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
        if (LAppDefine::DebugLogEnable)
        {
            LAppPal::PrintLogLn("[APP]delete buffer: %s", path);
        }
        LAppPal::ReleaseBytes(buffer);
    }
}

LAppMotionSyncLinkModel::LAppMotionSyncLinkModel()
    : CubismUserModel()
    , _modelSetting(NULL)
    , _userTimeSeconds(0)
    , _motionSync(NULL)
    , _motionIndex(-1)
    , _currentMotionSyncSettingIndex(0)
    , _isEnableMotionSync(false)
    , _currentMotionSyncMotion(NULL)
{
}

LAppMotionSyncLinkModel::~LAppMotionSyncLinkModel()
{
    ReleaseMotions();

    if (_motionSync)
    {
        _soundData.Release();
        CubismMotionSync::Delete(_motionSync);
    }

    delete _modelSetting;
}

void LAppMotionSyncLinkModel::LoadAssets(const csmString fileName)
{
    _modelHomeDir = csmString(ResourcesPath) + fileName + "/";

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
    SetupTextures();
}

void LAppMotionSyncLinkModel::Update()
{
    const csmFloat32 deltaTimeSeconds = LAppPal::GetDeltaTime();
    _userTimeSeconds += deltaTimeSeconds;

    _model->LoadParameters();
    _motionManager->UpdateMotion(_model, deltaTimeSeconds);
    _model->SaveParameters();

    _opacity = _model->GetModelOpacity();

    if (_physics != NULL)
    {
        _physics->Evaluate(_model, deltaTimeSeconds);
    }

    if (_pose != NULL)
    {
        _pose->UpdateParameters(_model, deltaTimeSeconds);
    }

    if (_motionSync != NULL)
    {
        _motionSync->UpdateParameters(_model, deltaTimeSeconds);
    }

    _model->Update();

    CGRect screenRect = [[UIScreen mainScreen] bounds];
    int width = screenRect.size.width;
    int height = screenRect.size.height;

    Csm::CubismMatrix44 projection;
    projection.LoadIdentity();

    if (_model->GetCanvasWidth() > 1.0f && width < height)
    {
        GetModelMatrix()->SetWidth(2.0f);
        projection.Scale(1.0f, static_cast<float>(width) / static_cast<float>(height));
    }
    else
    {
        projection.Scale(static_cast<float>(height) / static_cast<float>(width), 1.0f);
    }

    Draw(projection);
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
    csmString name = Utils::CubismString::GetFormatedString("%s_%d", group, no);
    CubismMotion* motion = static_cast<CubismMotion*>(_motions[name.GetRawString()]);
    csmBool autoDelete = false;

    if (motion == NULL)
    {
        csmString path = _modelHomeDir + fileName;

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
            autoDelete = true;
        }

        DeleteBuffer(buffer, path.GetRawString());
    }
    else
    {
        motion->SetBeganMotionHandler(onBeganMotionHandler);
        motion->SetFinishedMotionHandler(onFinishedMotionHandler);
    }

    if (!_motionSync || !_motionSyncSettingMap.IsExist(fileName) || !_soundFileMap.IsExist(fileName))
    {
        LAppPal::PrintLogLn("[APP]No exist: %s", fileName.GetRawString());
        return InvalidMotionQueueEntryHandleValue;
    }

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
    return _motionManager->StartMotionPriority(motion, autoDelete, priority);
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

    if (_motionSync && _currentMotionSyncSettingIndex >= 0)
    {
        _motionSync->SetSoundBuffer(_currentMotionSyncSettingIndex, NULL);
    }
    _soundData.Release();
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

    _isEnableMotionSync = true;

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

    if (strcmp(_modelSetting->GetModelFileName(), "") != 0)
    {
        csmString path = _modelHomeDir + _modelSetting->GetModelFileName();

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

    if (strcmp(_modelSetting->GetPhysicsFileName(), "") != 0)
    {
        csmString path = _modelHomeDir + _modelSetting->GetPhysicsFileName();

        buffer = CreateBuffer(path.GetRawString(), &size);
        LoadPhysics(buffer, size);
        DeleteBuffer(buffer, path.GetRawString());
    }

    if (strcmp(_modelSetting->GetPoseFileName(), "") != 0)
    {
        csmString path = _modelHomeDir + _modelSetting->GetPoseFileName();

        buffer = CreateBuffer(path.GetRawString(), &size);
        LoadPose(buffer, size);
        DeleteBuffer(buffer, path.GetRawString());
    }

    csmMap<csmString, csmFloat32> layout;
    _modelSetting->GetLayoutMap(layout);
    _modelMatrix->SetupFromLayout(layout);

    _model->SaveParameters();

    for (csmInt32 i = 0; i < _modelSetting->GetMotionGroupCount(); i++)
    {
        const csmChar* group = _modelSetting->GetMotionGroupName(i);
        PreloadMotionGroup(group);
    }

    _motionManager->StopAllMotions();

    const csmChar* fileName = _modelSetting->GetMotionSyncJsonFileName();

    if (strcmp(fileName, "") != 0)
    {
        if (_debugMode)
        {
            LAppPal::PrintLogLn("[APP]load motionSync setting: %s", fileName);
        }

        const csmString path = _modelHomeDir + fileName;
        buffer = CreateBuffer(path.GetRawString(), &size);

        _motionSync = CubismMotionSync::Create(_model, buffer, size, SamplesPerSec);

        if (!_motionSync)
        {
            LAppPal::PrintLogLn("Failed to SetupModel().");
            return;
        }

        DeleteBuffer(buffer, path.GetRawString());

        _modelSetting->GetMotionSyncLinkSettingMap(_motionSyncSettingMap);
        _modelSetting->GetMotionSyncLinkAudioMap(_soundFileMap);
        _motionIndex = -1;
    }

    _updating = false;
    _initialized = true;
}

void LAppMotionSyncLinkModel::SetupTextures()
{
    for (csmInt32 modelTextureNumber = 0; modelTextureNumber < _modelSetting->GetTextureCount(); modelTextureNumber++)
    {
        if (!strcmp(_modelSetting->GetTextureFileName(modelTextureNumber), ""))
        {
            continue;
        }

        csmString texturePath = _modelHomeDir + _modelSetting->GetTextureFileName(modelTextureNumber);

        AppMotionSyncLinkDelegate *delegate = (AppMotionSyncLinkDelegate *)[[UIApplication sharedApplication] delegate];
        TextureInfo* texture = [[delegate getTextureManager] createTextureFromPngFile:texturePath.GetRawString()];
        csmInt32 glTextueNumber = texture->id;

        GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->BindTexture(modelTextureNumber, glTextueNumber);
    }

    GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->IsPremultipliedAlpha(false);
}

void LAppMotionSyncLinkModel::PreloadMotionGroup(const Csm::csmChar* group)
{
    const csmInt32 count = _modelSetting->GetMotionCount(group);

    for (csmInt32 i = 0; i < count; i++)
    {
        csmString name = Utils::CubismString::GetFormatedString("%s_%d", group, i);
        csmString path = _modelHomeDir + _modelSetting->GetMotionFileName(group, i);

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

    matrix.MultiplyByMatrix(_modelMatrix);
    GetRenderer<Csm::Rendering::CubismRenderer_OpenGLES2>()->SetMvpMatrix(&matrix);
    GetRenderer<Csm::Rendering::CubismRenderer_OpenGLES2>()->DrawModel();
}
