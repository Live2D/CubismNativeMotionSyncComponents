/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "Motion/ACubismMotion.hpp"
#include "LAppMotionSyncLinkDelegate.hpp"
#include "LAppMotionSyncLinkView.hpp"


namespace LAppCubismMotionEventDefine {
    /**
     * @brief Callback function when motion playback finishes.
     *
     * @param self Instance of the finished motion.
     */
    void OnFinishedMotion(Csm::ACubismMotion* self)
    {
        LAppMotionSyncLinkDelegate* delegateInstance = LAppMotionSyncLinkDelegate::GetInstance();
        LAppMotionSyncLinkModel* model = delegateInstance->GetView()->GetCurrentModel();

        model->OnFinishedMotion(self);
    }
}
