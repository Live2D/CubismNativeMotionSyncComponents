/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LIVE2D_1FE7D7637C5F467DA22BF0137A413974_INCLUDED
#define LIVE2D_1FE7D7637C5F467DA22BF0137A413974_INCLUDED
#include <cstddef>

 /************************************************************************
  *               Cと互換性のある構造体として定義
  * Live2DCubismMotionSyncConfiguration_XXX.h と同じ構造体として定義すること。
  ************************************************************************/

  //TODO: [SK] 必要であれば有効にする
#if 0
struct MotionSyncEngineConfig_CRI
{
    MotionSyncEngineConfig_CRI(
        void* allocator = NULL
        , void* deallocator = NULL)
        : Allocator(allocator)
        , Deallocator(deallocator)
    {
    }

    /** Audio sample rate. */
    void* Allocator;

    /** Deallocator. */
    void* Deallocator;
};
#endif

/*
 * Context cofiguration for CRI.
 */
struct MotionSyncContextConfig_CRI
{
    MotionSyncContextConfig_CRI(
	    Csm::csmInt32 sampleRate = 0
        , Csm::csmInt32 bitDepth = 0)
        : SampleRate(sampleRate)
        , BitDepth(bitDepth)
    {
    }
    /** Audio sample rate. */
    Csm::csmInt32 SampleRate;

    /** Audio bit depth. */
    Csm::csmInt32 BitDepth;
};

/*
 * Analyze cofiguration for CRI.
 */
struct MotionSyncAnalysisConfig_CRI
{
    MotionSyncAnalysisConfig_CRI(
        Csm::csmFloat32 blendRatio = 0.0f
        , Csm::csmInt32 smoothing = 0
        , Csm::csmFloat32 audioLevelEffectRatio = 0.0f)
        : BlendRatio(blendRatio)
        , Smoothing(smoothing)
        , AudioLevelEffectRatio(audioLevelEffectRatio)
    {
    }

    /** Blending ratio for vieseme. */
    Csm::csmFloat32 BlendRatio;

    /** Smoothing value. */
    Csm::csmInt32 Smoothing;

    /** Audio level effect ratio. */
    /** Unused */
    Csm::csmFloat32 AudioLevelEffectRatio;
};

#endif // LIVE2D_1FE7D7637C5F467DA22BF0137A413974_INCLUDED
