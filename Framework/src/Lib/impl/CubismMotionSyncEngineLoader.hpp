/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LIVE2D_C3B887DF018044E384CCF1FFF892970A_INCLUDED
#define LIVE2D_C3B887DF018044E384CCF1FFF892970A_INCLUDED

#include "Live2DCubismMotionSyncEngine.h"
#include "CubismFramework.hpp"

#ifdef _WIN32
#ifndef _WIN64
#define csmMotionSyncCallingPtr __stdcall
#endif
#endif

#ifndef csmMotionSyncCallingPtr
#define csmMotionSyncCallingPtr
#endif

class LibraryLoader;

typedef csmMotionSyncVersion(csmMotionSyncCallingPtr *csmMotionSync_GetEngineVersionFunc)();

typedef const Csm::csmChar* (csmMotionSyncCallingPtr *csmMotionSync_GetEngineNameFunc)();

typedef Csm::csmInt32 (csmMotionSyncCallingPtr *csmMotionSync_InitializeEngineFunc)(
    const void* engineConfiguration
    );

typedef void (csmMotionSyncCallingPtr *csmMotionSync_DisposeEngineFunc)();

typedef csmMotionSync_Context(csmMotionSyncCallingPtr *csmMotionSync_CreateContextFunc)(
    const void* contextConfiguration,
    const csmMotionSync_MappingInfo* mappingInformations,
    Csm::csmInt32 mappingInformationCount
    );

typedef void (csmMotionSyncCallingPtr *csmMotionSync_ClearContextFunc)(
    csmMotionSync_Context context
    );

typedef void (csmMotionSyncCallingPtr *csmMotionSync_DeleteContextFunc)(
    csmMotionSync_Context context
    );

typedef Csm::csmUint32 (csmMotionSyncCallingPtr *csmMotionSync_GetRequireSampleCountFunc)(
    csmMotionSync_Context context
    );

typedef Csm::csmInt32 (csmMotionSyncCallingPtr *csmMotionSync_AnalyzeFunc)(
    csmMotionSync_Context context,
    const Csm::csmFloat32* samples,
    Csm::csmUint32 sampleCount,
    csmMotionSync_AnalysisResult* analysisResult,
    const void* analysisConfig
    );

class CubismMotionSyncEngineLoader
{
public:
    CubismMotionSyncEngineLoader();
    ~CubismMotionSyncEngineLoader();
    csmMotionSync_GetEngineVersionFunc GetEngineVersion;
    csmMotionSync_GetEngineNameFunc GetEngineName;
    csmMotionSync_InitializeEngineFunc InitializeEngine;
    csmMotionSync_DisposeEngineFunc DisposeEngine;
    csmMotionSync_CreateContextFunc CreateContext;
    csmMotionSync_ClearContextFunc ClearContext;
    csmMotionSync_DeleteContextFunc DeleteContext;
    csmMotionSync_GetRequireSampleCountFunc GetRequireSampleCount;
    csmMotionSync_AnalyzeFunc Analyze;

    Csm::csmBool isValid() const;

    Csm::csmBool LoadLib(const Csm::csmChar* dllPath);
    void UnloadLib();

private:
    void ResetFuncPtr();

    LibraryLoader* _loader;
};

#endif // LIVE2D_C3B887DF018044E384CCF1FFF892970A_INCLUDED
