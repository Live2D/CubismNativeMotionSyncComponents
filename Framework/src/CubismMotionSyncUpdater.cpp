/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncUpdater.hpp"

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

CubismMotionSyncUpdater::CubismMotionSyncUpdater(CubismMotionSync& motionSync)
    : ICubismUpdater(CubismUpdateOrder_MotionSync)
    , _motionSync(motionSync)
{
}

CubismMotionSyncUpdater::CubismMotionSyncUpdater(CubismMotionSync& motionSync, const csmInt32 executionOrder)
    : ICubismUpdater(executionOrder)
    , _motionSync(motionSync)
{
}

void CubismMotionSyncUpdater::OnLateUpdate(CubismModel* model, const csmFloat32 deltaTimeSeconds)
{
    if (model == nullptr)
    {
        return;
    }

    _motionSync.UpdateParameters(model, deltaTimeSeconds);
}

}}}}
