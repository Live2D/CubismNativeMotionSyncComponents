/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "ICubismMotionSyncProcessor.hpp"
#include "ICubismMotionSyncEngine.hpp"

 //--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

CubismMotionSyncEngineAnalysisResult* ICubismMotionSyncProcessor::CreateAnalysisResult()
{
    if (IsClosed() || _mappingInfoArray.GetSize() == 0)
    {
        return CSM_NEW CubismMotionSyncEngineAnalysisResult(0);
    }

    return CSM_NEW CubismMotionSyncEngineAnalysisResult(_mappingInfoArray[0].GetModelParameterValues().GetSize());
}

Framework::csmVector<CubismMotionSyncEngineMappingInfo> ICubismMotionSyncProcessor::GetMappingInfoArray()
{
    return _mappingInfoArray;
}

EngineType ICubismMotionSyncProcessor::GetType()
{
    return _engine->GetType();
}

csmUint32 ICubismMotionSyncProcessor::GetRequireSampleCount()
{
    return _engine->GetEngineHandle()->GetRequireSampleCount(GetContextHandle()->Context());
}

Framework::csmBool ICubismMotionSyncProcessor::IsClosed()
{
    return _contextHandle == NULL;
}

void ICubismMotionSyncProcessor::Close()
{
    // 解放済みなら何もしない。
    if (_contextHandle == NULL)
    {
        return;
    }

    _engine->GetEngineHandle()->DeleteContext(_contextHandle->Context());
    CSM_DELETE(_contextHandle);
    _contextHandle = NULL;
    _engine->DeleteAssociation(this);
}

MotionSyncContext* ICubismMotionSyncProcessor::GetContextHandle()
{
    return _contextHandle;
}

ICubismMotionSyncProcessor::ICubismMotionSyncProcessor(ICubismMotionSyncEngine *engine, MotionSyncContext *contextHandle, Framework::csmVector<CubismMotionSyncEngineMappingInfo> mappingInfoArray) :
    _engine(engine),
    _contextHandle(contextHandle),
    _mappingInfoArray(mappingInfoArray)
{
}

ICubismMotionSyncProcessor::~ICubismMotionSyncProcessor()
{
}

}}}}
