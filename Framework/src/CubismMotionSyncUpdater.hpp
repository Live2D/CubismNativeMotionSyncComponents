/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "Motion/ICubismUpdater.hpp"
#include "CubismMotionSync.hpp"

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

// Motion sync calculation order.
constexpr csmInt32 CubismUpdateOrder_MotionSync = (CubismUpdateOrder_LipSync + CubismUpdateOrder_Pose) / 2;

/**
 * Class for updating motion sync.<br>
 * Handles the management of motion playback through the CubismUpdateScheduler.
 */
class CubismMotionSyncUpdater : public ICubismUpdater
{
public:
    /**
     * Delete constructor
     */
    CubismMotionSyncUpdater() = delete;

    /**
     * Constructor
     *
     * @param motionSync `CubismMotionSync` reference
     */
    CubismMotionSyncUpdater(CubismMotionSync& motionSync);

    /**
     * Constructor
     *
     * @param motionSync `CubismMotionSync` reference
     * @param executionOrder Order of operations
     */
    CubismMotionSyncUpdater(CubismMotionSync& motionSync, csmInt32 executionOrder);

    /**
     * Destructor
     */
    virtual ~CubismMotionSyncUpdater() = default;

    /**
     * Update process.
     *
     * @param model Model to update
     * @param deltaTimeSeconds Delta time in seconds.
     */
    virtual void OnLateUpdate(CubismModel* model, csmFloat32 deltaTimeSeconds) override;

private:
    CubismMotionSync& _motionSync;
};

}}}}
