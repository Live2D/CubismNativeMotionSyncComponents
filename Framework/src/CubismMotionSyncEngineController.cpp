/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

 // Apple Privacy Manifestsの影響のため無効化
#define USE_STAT_LIB 0

#include "CubismMotionSyncEngineController.hpp"
#if USE_STAT_LIB
#include <sys/stat.h>
#endif
#include "CubismMotionSyncEngineVersion.hpp"
#include "CubismMotionSyncEngineCri.hpp"
#ifndef CSM_MOTIONSYNC_USE_STATIC_LIB
#include "Lib/CubismMotionSyncEngineLib.hpp"
#else
#include "Lib/CubismMotionSyncEngineStaticLib.hpp"
#endif

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

csmMap<EngineType, ICubismMotionSyncEngine*> CubismMotionSyncEngineController::_engineMap;

ICubismMotionSyncEngine* CubismMotionSyncEngineController::InstallEngine(csmString dllFilePath)
{
#ifndef CSM_MOTIONSYNC_USE_STATIC_LIB
    // 動的ライブラリの使用

#if USE_STAT_LIB
    struct stat statBuf;

    // 渡されたパスにファイルが存在するかチェック
    if (stat(dllFilePath.GetRawString(), &statBuf) != 0)
    {
        return NULL;
    }
#endif

    ICubismMotionSyncEngineLib* engineHandle = CSM_NEW CubismMotionSyncEngineLib();
    if (!engineHandle->LoadLibrary(dllFilePath.GetRawString()))
    {
        CSM_DELETE(engineHandle);
        return NULL;
    }
#else
    // 静的ライブラリの使用

    ICubismMotionSyncEngineLib* engineHandle = CSM_NEW CubismMotionSyncEngineStaticLib();
#endif
    
    csmString engineName = engineHandle->GetEngineName();
    EngineType engineType = ToEngineType(engineName);
    csmInt32 nativeVersion = engineHandle->GetEngineVersion();
    CubismMotionSyncEngineVersion version = CubismMotionSyncEngineVersion(nativeVersion);

    // 同じ種類のエンジンが2回以上呼ばれた場合はunloadEngine()を行う。
    if (_engineMap.IsExist(engineType))
    {
        engineHandle->UnloadLibrary();
        CSM_DELETE(engineHandle);
        return NULL;
    }
    // 初期化
    csmInt32 isInitialized = engineHandle->InitializeEngine(NULL);
    if (isInitialized == 0)
    {
        //unloadEngineを呼びだす
        engineHandle->UnloadLibrary();
        CSM_DELETE(engineHandle);
        return NULL;
    }

    ICubismMotionSyncEngine *engine = NULL;
    switch (engineType)
    {
    case EngineType_Cri:
        engine = CSM_NEW CubismMotionSyncEngineCri(engineHandle, engineType, engineName, version);
        break;
    default:
        engineHandle->DisposeEngine();
        engineHandle->UnloadLibrary();
        CSM_DELETE(engineHandle);
        return NULL;
    }
    _engineMap[engineType] = engine;

    return engine;
}

ICubismMotionSyncEngine* CubismMotionSyncEngineController::GetEngine(EngineType type)
{
    if (_engineMap.IsExist(type))
    {
        return _engineMap[type];
    }
    
    return NULL;
}

csmVector<ICubismMotionSyncEngine*> CubismMotionSyncEngineController::GetEngines()
{
    csmVector<ICubismMotionSyncEngine*> vector;

    for (csmMap<EngineType, ICubismMotionSyncEngine*>::const_iterator iter = _engineMap.Begin(); iter != _engineMap.End(); iter++)
    {
        vector.PushBack(iter->Second);
    }
    return vector;
}

void CubismMotionSyncEngineController::ReleaseEngine(ICubismMotionSyncEngine *engine)
{
    ReleaseEngine(engine, false);
}

void CubismMotionSyncEngineController::ReleaseEngine(ICubismMotionSyncEngine *engine, csmBool isForce)
{
    engine->Close(isForce);
}

void CubismMotionSyncEngineController::DeleteAssociation(ICubismMotionSyncEngine *engine)
{
    for (csmMap<EngineType, ICubismMotionSyncEngine*>::const_iterator iter = _engineMap.Begin(); iter != _engineMap.End(); iter++)
    {
        if (iter->First == engine->GetType())
        {
            CSM_DELETE(engine);
            _engineMap.Erase(iter);
            break;
        }
    }
}

void CubismMotionSyncEngineController::DeleteAllEngine()
{
    csmVector<ICubismMotionSyncEngine*> engines = GetEngines();

    for (csmUint32 i = 0; i < engines.GetSize(); i++)
    {
        engines[i]->Close(true);
    }

    _engineMap.Clear();
}


EngineType CubismMotionSyncEngineController::ToEngineType(csmString engineName)
{
    EngineType engineType = EngineType_Unknown;

    if (engineName == "Live2DCubismMotionSyncEngine_CRI")
    {
        engineType = EngineType_Cri;
    }

    return engineType;
}

CubismMotionSyncEngineController::CubismMotionSyncEngineController()
{ }

CubismMotionSyncEngineController::~CubismMotionSyncEngineController()
{ }

}}}}
