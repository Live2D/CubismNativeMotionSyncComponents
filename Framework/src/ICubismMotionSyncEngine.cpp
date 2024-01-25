/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "ICubismMotionSyncEngine.hpp"
#include "ICubismMotionSyncProcessor.hpp"
#include "CubismMotionSyncEngineController.hpp"

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

EngineType ICubismMotionSyncEngine::GetType()
{
    return _type;
}

csmString ICubismMotionSyncEngine::GetName()
{
    return _name;
}

CubismMotionSyncEngineVersion ICubismMotionSyncEngine::GetVersion()
{
    return _version;
}

csmBool ICubismMotionSyncEngine::IsClosed()
{
    return _engineHandle == NULL;
}

csmVector<ICubismMotionSyncProcessor*> ICubismMotionSyncEngine::GetProcessors()
{
    return _processors;
}

void ICubismMotionSyncEngine::ReleaseAllProcessor()
{
    if (IsClosed())
    {
        return;
    }

    // Close内でDeleteAssociationが呼ばれている
    csmVector<ICubismMotionSyncProcessor*> deleteProcessors = _processors;

    for (csmUint32 i = 0; i < deleteProcessors.GetSize(); i++)
    {
        deleteProcessors[i]->Close();
    }
}

ICubismMotionSyncEngineLib* ICubismMotionSyncEngine::GetEngineHandle()
{
    return _engineHandle;
}

void ICubismMotionSyncEngine::Close(csmBool isForce)
{
    if (IsClosed())
    {
        return;
    }

    if (0 < _processors.GetSize())
    {
        if (isForce)
        {
            ReleaseAllProcessor();
        }
        else
        {
            return;
        }
    }

    _engineHandle->DisposeEngine();
    _engineHandle->UnloadLibrary();
    CSM_DELETE(_engineHandle);
    _engineHandle = NULL;
    CubismMotionSyncEngineController::DeleteAssociation(this);
}

void ICubismMotionSyncEngine::DeleteAssociation(ICubismMotionSyncProcessor *processor)
{
    for (csmUint32 i = 0; i < _processors.GetSize(); i++)
    {
        if (_processors[i] == processor)
        {
            CSM_DELETE(_processors[i]);
            _processors.Remove(i);
            break;
        }
    }
}

ICubismMotionSyncEngine::ICubismMotionSyncEngine(ICubismMotionSyncEngineLib *engineHandle, EngineType type, csmString name, CubismMotionSyncEngineVersion version) :
    _engineHandle(engineHandle),
    _type(type),
    _name(name),
    _version(version)
{
}

ICubismMotionSyncEngine::~ICubismMotionSyncEngine()
{
}

}}}}
