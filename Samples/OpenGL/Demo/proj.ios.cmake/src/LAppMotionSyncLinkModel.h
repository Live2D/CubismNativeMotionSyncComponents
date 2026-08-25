/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#import <CubismFramework.hpp>
#import <Model/CubismUserModel.hpp>
#import <Motion/CubismMotion.hpp>
#import "CubismMotionSync.hpp"
#import "CubismModelMotionSyncSettingJson.hpp"
#import "LAppAudioManager.h"

class LAppMotionSyncLinkModel : public Csm::CubismUserModel
{
public:
    LAppMotionSyncLinkModel();
    virtual ~LAppMotionSyncLinkModel();

    void LoadAssets(const Csm::csmString fileName);
    void Update();
    void ChangeNextMotion();

    Csm::CubismMotionQueueEntryHandle StartMotion(const Csm::csmChar* group, Csm::csmInt32 no, Csm::csmInt32 priority, Csm::ACubismMotion::FinishedMotionCallback onFinishedMotionHandler = NULL, Csm::ACubismMotion::BeganMotionCallback onBeganMotionHandler = NULL);
    void OnFinishedMotion(Csm::ACubismMotion* self);

private:
    Csm::MotionSync::CubismModelMotionSyncSettingJson* _modelSetting;
    Csm::csmString _modelHomeDir;
    Csm::csmFloat32 _userTimeSeconds;
    Csm::MotionSync::CubismMotionSync* _motionSync;
    LAppAudioManager _soundData;
    Csm::csmMap<Csm::csmString, Csm::csmString> _motionSyncSettingMap;
    Csm::csmMap<Csm::csmString, Csm::csmString> _soundFileMap;
    Csm::csmInt32 _motionIndex;
    Csm::csmMap<Csm::csmString, Csm::ACubismMotion*> _motions;
    Csm::csmInt32 _currentMotionSyncSettingIndex;
    Csm::csmString _currentSoundFileName;
    Csm::csmBool _isEnableMotionSync;
    Csm::ACubismMotion* _currentMotionSyncMotion; ///< 現在のモーションシンクを再生しているモーション

    void StartMotionSync();
    void SetupModel();
    void SetupTextures();
    void PreloadMotionGroup(const Csm::csmChar* group);
    void ReleaseMotions();
    void Draw(Csm::CubismMatrix44& matrix);
};
