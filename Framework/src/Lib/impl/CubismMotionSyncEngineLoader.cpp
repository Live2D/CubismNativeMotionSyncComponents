/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncEngineLoader.hpp"
#include "LibraryLoader.h"

CubismMotionSyncEngineLoader::CubismMotionSyncEngineLoader()
{
    _loader = CSM_NEW LibraryLoader;
    ResetFuncPtr();
}

CubismMotionSyncEngineLoader::~CubismMotionSyncEngineLoader()
{
    UnloadLib();

    CSM_DELETE(_loader);
    _loader = NULL;
}

Csm::csmBool CubismMotionSyncEngineLoader::isValid() const
{
    return _loader->isValid();
}

Csm::csmBool CubismMotionSyncEngineLoader::LoadLib(const Csm::csmChar* dllPath)
{
    //DLLロード済みの場合
    if (isValid())
    {
        return true;
    }

    const Csm::csmBool isLoadSucceed = _loader->Load(dllPath);
    if (!isLoadSucceed)
    {
        return false;
    }

    GetEngineVersion = _loader->GetFuncPtr<csmMotionSync_GetEngineVersionFunc>("csmMotionSync_GetEngineVersion");
    GetEngineName = _loader->GetFuncPtr<csmMotionSync_GetEngineNameFunc>("csmMotionSync_GetEngineName");
    InitializeEngine = _loader->GetFuncPtr<csmMotionSync_InitializeEngineFunc>("csmMotionSync_InitializeEngine");
    DisposeEngine = _loader->GetFuncPtr<csmMotionSync_DisposeEngineFunc>("csmMotionSync_DisposeEngine");
    CreateContext = _loader->GetFuncPtr<csmMotionSync_CreateContextFunc>("csmMotionSync_CreateContext");
    ClearContext = _loader->GetFuncPtr<csmMotionSync_ClearContextFunc>("csmMotionSync_ClearContext");
    DeleteContext = _loader->GetFuncPtr<csmMotionSync_DeleteContextFunc>("csmMotionSync_DeleteContext");
    GetRequireSampleCount = _loader->GetFuncPtr<csmMotionSync_GetRequireSampleCountFunc>("csmMotionSync_GetRequireSampleCount");
    Analyze = _loader->GetFuncPtr<csmMotionSync_AnalyzeFunc>("csmMotionSync_Analyze");

    const Csm::csmBool isFuncValid =
        GetEngineVersion && GetEngineName && InitializeEngine
        && DisposeEngine && CreateContext
        && ClearContext && DeleteContext
        && GetRequireSampleCount && Analyze;

    if (!isFuncValid)
    {
        UnloadLib();
        return false;
    }

    return true;
}

void CubismMotionSyncEngineLoader::UnloadLib()
{
    if (!isValid())
    {
        return;
    }

    ResetFuncPtr();
    _loader->Unload();
}

void CubismMotionSyncEngineLoader::ResetFuncPtr()
{
    GetEngineVersion = NULL;
    GetEngineName = NULL;
    InitializeEngine = NULL;
    DisposeEngine = NULL;
    CreateContext = NULL;
    ClearContext = NULL;
    DeleteContext = NULL;
    GetRequireSampleCount = NULL;
    Analyze = NULL;
}
