/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncEngineAnalysisResult.hpp"
#include "CubismFramework.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {
csmFloat32* CubismMotionSyncEngineAnalysisResult::GetValues()
{
    return _values;
}

csmInt32 CubismMotionSyncEngineAnalysisResult::GetValuesCount()
{
    return _valuesCount;
}

void CubismMotionSyncEngineAnalysisResult::SetProcessedSampleCount(csmInt32 sampleCount)
{
    _processedSampleCount = sampleCount;
}

csmInt32 CubismMotionSyncEngineAnalysisResult::GetProcessedSampleCount()
{
    return _processedSampleCount;
}


CubismMotionSyncEngineAnalysisResult::CubismMotionSyncEngineAnalysisResult(csmInt32 valuesSize)
{
    _values = static_cast<csmFloat32*>(CSM_MALLOC(sizeof(csmFloat32) * valuesSize));
    _valuesCount = valuesSize;
    _processedSampleCount = 0;
}

CubismMotionSyncEngineAnalysisResult::CubismMotionSyncEngineAnalysisResult(const CubismMotionSyncEngineAnalysisResult &result)
{
    _values = static_cast<csmFloat32*>(CSM_MALLOC(sizeof(csmFloat32) * result._valuesCount));
    for (csmInt32 i = 0; i < result._valuesCount; i++)
    {
        _values[i] = result._values[i];
    }
    _valuesCount = result._valuesCount;
    _processedSampleCount = result._processedSampleCount;
}

CubismMotionSyncEngineAnalysisResult::~CubismMotionSyncEngineAnalysisResult()
{
    CSM_FREE(_values);
}

}}}}
