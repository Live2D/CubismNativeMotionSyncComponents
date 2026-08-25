/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#import "Motion/ACubismMotion.hpp"
#import "AppMotionSyncLinkDelegate.h"
#import "MotionViewController.h"

namespace LAppCubismMotionEventDefine {
    static inline void OnFinishedMotion(Csm::ACubismMotion* self)
    {
        AppMotionSyncLinkDelegate* delegate = (AppMotionSyncLinkDelegate*)[[UIApplication sharedApplication] delegate];
        LAppMotionSyncLinkModel* model = [delegate.viewController getCurrentModel];

        if (model)
        {
            model->OnFinishedMotion(self);
        }
    }
}
