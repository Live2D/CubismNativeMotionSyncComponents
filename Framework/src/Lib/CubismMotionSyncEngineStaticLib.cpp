/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncEngineStaticLib.hpp"
#include "Live2DCubismMotionSyncEngine.hpp"

using namespace Live2D::Cubism::Framework::MotionSync;

inline Csm::csmBool ConvertMotionSyncBool(Csm::csmInt32 value)
{
    if (value == StaticLib::csmMotionSyncTrue)
    {
        return true;
    }
    else
    {
        return false;
    }
}

CubismMotionSyncEngineStaticLib::CubismMotionSyncEngineStaticLib() :
    ICubismMotionSyncEngineLib()
{
}

CubismMotionSyncEngineStaticLib::~CubismMotionSyncEngineStaticLib()
{
    UnloadLibrary();
}

CubismMotionSyncVersion CubismMotionSyncEngineStaticLib::GetEngineVersion()
{
    return StaticLib::csmMotionSync_GetEngineVersion();
}

const Csm::csmChar* CubismMotionSyncEngineStaticLib::GetEngineName()
{
    return StaticLib::csmMotionSync_GetEngineName();
}

Csm::csmBool CubismMotionSyncEngineStaticLib::InitializeEngine(const void* engineConfig)
{
    const int ret = StaticLib::csmMotionSync_InitializeEngine(engineConfig);
    return ConvertMotionSyncBool(ret);
}

void CubismMotionSyncEngineStaticLib::DisposeEngine()
{
    StaticLib::csmMotionSync_DisposeEngine();
}

CubismMotionSyncContext CubismMotionSyncEngineStaticLib::CreateContext(
    const void* contextConfig
    , const CubismMotionSyncMappingInfo* mappingInfoList
    , Csm::csmInt32 mappingInfoListCount
)
{
    return StaticLib::csmMotionSync_CreateContext(
        contextConfig
        , reinterpret_cast<const StaticLib::csmMotionSync_MappingInfo*>(mappingInfoList)
        , mappingInfoListCount
    );
}

void CubismMotionSyncEngineStaticLib::ClearContext(CubismMotionSyncContext context)
{
    StaticLib::csmMotionSync_ClearContext(context);
}

void CubismMotionSyncEngineStaticLib::DeleteContext(CubismMotionSyncContext context)
{
    StaticLib::csmMotionSync_DeleteContext(context);
}

Csm::csmUint32 CubismMotionSyncEngineStaticLib::GetRequireSampleCount(CubismMotionSyncContext context)
{
    return StaticLib::csmMotionSync_GetRequireSampleCount(context);
}

Csm::csmBool CubismMotionSyncEngineStaticLib::Analyze(
    CubismMotionSyncContext context
    , const Csm::csmFloat32* samples
    , Csm::csmUint32 sampleCount
    , CubismMotionSyncAnalysisResult* analysisResult
    , const void* analyzeConfig
)
{
    const Csm::csmInt32 ret = StaticLib::csmMotionSync_Analyze(
        context
        , samples
        , sampleCount
        , reinterpret_cast<StaticLib::csmMotionSync_AnalysisResult*>(analysisResult)
        , analyzeConfig
    );

    return ConvertMotionSyncBool(ret);
}
