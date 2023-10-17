﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncProcessorCRI.hpp"
#include "Lib/CubismMotionSyncConfig.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

csmInt32 CubismMotionSyncProcessorCRI::GetSampleRate()
{
    return _sampleRate;
}

csmInt32 CubismMotionSyncProcessorCRI::GetBitDepth()
{
    return _bitDepth;
}

CubismMotionSyncEngineAnalysisResult* CubismMotionSyncProcessorCRI::Analyze(csmVector<csmFloat32> &samples, csmInt32 beginIndex, csmFloat32 blendRatio, csmInt32 smoothing, csmFloat32 audioLevelEffectRatio, CubismMotionSyncEngineAnalysisResult *result)
{
    if (IsClosed())
    {
        return NULL;
    }

    if (samples.GetSize() == 0)
    {
        CubismLogError("The argument is invalid. Please check that the samples is the correct value.");
        return NULL;
    }
    if (!(0 <= beginIndex && beginIndex < samples.GetSize()))
    {
        CubismLogError("The value of beginIndex is incorrect. It should be less than the length of samples.");
        return NULL;
    }
    if (!(0.0f <= blendRatio && blendRatio <= 1.0f))
    {
        CubismLogError("The value of blend ratio is incorrect. The value is limited to between 0.0 and 1.0.");
    }
    if (!(1 <= smoothing && smoothing <= 100))
    {
        CubismLogError("The value of smoothing is incorrect. The value is limited to between 1 and 100.");
        return NULL;
    }
    if (!(0.0f <= audioLevelEffectRatio && audioLevelEffectRatio <= 1.0f))
    {
        CubismLogError("The value of audio level effect ratio is incorrect. The value is limited to between 0.0 and 1.0.");
        return NULL;
    }
    if (result == NULL)
    {
        CubismLogError("The result instance is null.");
        return NULL;
    }

    const csmInt32 samplesCount = samples.GetSize();

    CubismMotionSyncAnalysisResult analysisResult(result->GetValues(), result->GetValuesCount(), 0);
    MotionSyncAnalysisConfig_CRI analysisConfig(blendRatio, smoothing, audioLevelEffectRatio);

    const csmBool ret = _engine->GetEngineHandle()->Analyze(GetContextHandle()->Context(), samples.GetPtr() + beginIndex, samplesCount - beginIndex, &analysisResult, &analysisConfig);

    result->SetProcessedSampleCount(analysisResult.ProcessedSampleCount);

    if (!ret)
    {
        return NULL;
    }

    return result;
}

CubismMotionSyncEngineAnalysisResult* CubismMotionSyncProcessorCRI::Analyze(csmVector<csmFloat32> &samples, csmInt32 beginIndex, csmFloat32 blendRatio, csmInt32 smoothing, csmFloat32 audioLevelEffectRatio)
{
    CubismMotionSyncEngineAnalysisResult *result = CreateAnalysisResult();
    Analyze(samples, beginIndex, blendRatio, smoothing, audioLevelEffectRatio, result);
    return result;
}

CubismMotionSyncProcessorCRI::CubismMotionSyncProcessorCRI(ICubismMotionSyncEngine *engine, MotionSyncContext *contextHandle, csmVector<CubismMotionSyncEngineMappingInfo> mappingInfoArray, csmInt32 sampleRate, csmInt32 bitDepth) :
    ICubismMotionSyncProcessor(engine, contextHandle, mappingInfoArray),
    _sampleRate(sampleRate),
    _bitDepth(bitDepth)
{
}

CubismMotionSyncProcessorCRI::~CubismMotionSyncProcessorCRI()
{
}

}}}}
