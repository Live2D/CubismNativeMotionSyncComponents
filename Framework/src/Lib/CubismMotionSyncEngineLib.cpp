/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncEngineLib.hpp"
#include <cstddef>
#include "CubismFramework.hpp"
#include "impl/CubismMotionSyncEngineLoader.hpp"

#define CSM_ENSURE(COND) \
    if (!(COND)) { return; } else {}
#define CSM_ENSURE2(COND, RET_VALUE) \
    if (!(COND)) { return RET_VALUE; } else {}

inline Csm::csmBool ConvertMotionSyncBool(Csm::csmInt32 value)
{
    if (value == csmMotionSyncTrue)
    {
        return true;
    }
    else
    {
        return false;
    }
}

CubismMotionSyncEngineLib::CubismMotionSyncEngineLib() :
    ICubismMotionSyncEngineLib()
{
    _engineIf = CSM_NEW CubismMotionSyncEngineLoader();
}

CubismMotionSyncEngineLib::~CubismMotionSyncEngineLib()
{
    UnloadLibrary();

    CSM_DELETE(_engineIf);
    _engineIf = NULL;
}

Csm::csmBool CubismMotionSyncEngineLib::LoadLibrary(const Csm::csmChar* dllPath)
{
    //既にロード済みの場合は何もせずに返る
    CSM_ENSURE2(!_engineIf->isValid(), true);
    return _engineIf->LoadLib(dllPath);
}

void CubismMotionSyncEngineLib::UnloadLibrary()
{
    CSM_ENSURE(_engineIf->isValid());
    _engineIf->UnloadLib();
}

CubismMotionSyncVersion CubismMotionSyncEngineLib::GetEngineVersion()
{
    CSM_ENSURE2(_engineIf->isValid(), 0);
    return _engineIf->GetEngineVersion();
}

const Csm::csmChar* CubismMotionSyncEngineLib::GetEngineName()
{
    CSM_ENSURE2(_engineIf->isValid(), "");
    return _engineIf->GetEngineName();
}

Csm::csmBool CubismMotionSyncEngineLib::InitializeEngine(const void* engineConfig)
{
    CSM_ENSURE2(_engineIf->isValid(), false);
    const int ret = _engineIf->InitializeEngine(engineConfig);
    return ConvertMotionSyncBool(ret);
}

void CubismMotionSyncEngineLib::DisposeEngine()
{
    CSM_ENSURE(_engineIf->isValid());
    _engineIf->DisposeEngine();
}

CubismMotionSyncContext CubismMotionSyncEngineLib::CreateContext(
    const void* contextConfig
    , const CubismMotionSyncMappingInfo* mappingInfoList
    , Csm::csmInt32 mappingInfoListCount
)
{
    CSM_ENSURE2(_engineIf->isValid(), NULL);
    return _engineIf->CreateContext(
        contextConfig
        , reinterpret_cast<const csmMotionSync_MappingInfo*>(mappingInfoList)
        , mappingInfoListCount
    );
}

void CubismMotionSyncEngineLib::ClearContext(CubismMotionSyncContext context)
{
    CSM_ENSURE(_engineIf->isValid());
    _engineIf->ClearContext(context);
}

void CubismMotionSyncEngineLib::DeleteContext(CubismMotionSyncContext context)
{
    CSM_ENSURE(_engineIf->isValid());
    _engineIf->DeleteContext(context);
}

Csm::csmUint32 CubismMotionSyncEngineLib::GetRequireSampleCount(CubismMotionSyncContext context)
{
    CSM_ENSURE2(_engineIf->isValid(), 0);
    return _engineIf->GetRequireSampleCount(context);
}

Csm::csmBool CubismMotionSyncEngineLib::Analyze(
    CubismMotionSyncContext context
    , const Csm::csmFloat32* samples
    , Csm::csmUint32 sampleCount
    , CubismMotionSyncAnalysisResult* analysisResult
    , const void* analyzeConfig
)
{
    CSM_ENSURE2(_engineIf->isValid(), false);
    const Csm::csmInt32 ret = _engineIf->Analyze(
        context
        , samples
        , sampleCount
        , reinterpret_cast<csmMotionSync_AnalysisResult*>(analysisResult)
        , analyzeConfig
    );

    return ConvertMotionSyncBool(ret);
}
