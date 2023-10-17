/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LibraryLoader.h"

#include <dlfcn.h>

#define DYNAMIC_LIBRARY_LOAD(X) dlopen(X,RTLD_NOW)
#define DYNAMIC_LIBRARY_INSTANCE void*
#define DYNAMIC_LIBRARY_FREE dlclose
#define DYNAMIC_LIBRARY_LOAD_PROC dlsym


bool LibraryLoader::Load(const char* dllPath)
{
    //DLLロード済みの場合
    if (isValid())
    {
        return true;
    }

    module_ = DYNAMIC_LIBRARY_LOAD(dllPath);

    return module_;
}

void LibraryLoader::Unload()
{
    if (!isValid())
    {
        return;
    }

    DYNAMIC_LIBRARY_FREE((DYNAMIC_LIBRARY_INSTANCE)module_);
    module_ = NULL;
}

void* LibraryLoader::GetFuncPtrInternal(const char* funcName)
{
    if (!isValid())
    {
        return NULL;
    }
    return DYNAMIC_LIBRARY_LOAD_PROC(module_, funcName);
}
