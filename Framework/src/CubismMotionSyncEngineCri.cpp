﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncEngineCri.hpp"
#include "Lib/CubismMotionSyncConfig.hpp"

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {
namespace
{
csmUint32 SampleRateMin = 16000;
csmUint32 SampleRateMax = 128000;
}

CubismMotionSyncProcessorCRI* CubismMotionSyncEngineCri::CreateProcessor(csmVector<CubismMotionSyncEngineMappingInfo> mappingInfoArray, csmInt32 sampleRate)
{
    if (IsClosed())
    {
        return NULL;
    }

    if (mappingInfoArray.GetSize() < 6)
    {
        return NULL;
    }
    if (!(SampleRateMin <= sampleRate && sampleRate <= SampleRateMax))
    {
        return NULL;
    }

    const MotionSyncContextConfig_CRI contextConfig(sampleRate, DEFAULT_AUDIO_BIT_DEPTH);
    MappingInfoListMapper* const mapper = CSM_NEW MappingInfoListMapper();
    mapper->SetJObject(mappingInfoArray);

    CubismMotionSyncContext context = GetEngineHandle()->CreateContext(&contextConfig, mapper->Data(), mapper->Count());
    if (context == NULL)
    {
        CubismLogError("Failed to create a motion sync processor. Make sure there are no problems with the arguments.");
        CSM_DELETE(mapper);
        return NULL;
    }
    MotionSyncContext* contextHandle = CSM_NEW MotionSyncContext(context, mapper);

    CubismMotionSyncProcessorCRI *processor = CSM_NEW CubismMotionSyncProcessorCRI(this, contextHandle, mappingInfoArray, sampleRate, DEFAULT_AUDIO_BIT_DEPTH);
    _processors.PushBack(processor);

    return processor;
}

CubismMotionSyncEngineCri::CubismMotionSyncEngineCri(CubismMotionSyncEngineLib* engineHandle, EngineType type, csmString name, CubismMotionSyncEngineVersion version) :
    ICubismMotionSyncEngine(engineHandle, type, name, version)
{
}

CubismMotionSyncEngineCri::~CubismMotionSyncEngineCri()
{
}

}}}}
