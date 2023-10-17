/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LIVE2D_0BA1D179C7E84EEE9FBFD714C7B87282_INCLUDED
#define LIVE2D_0BA1D179C7E84EEE9FBFD714C7B87282_INCLUDED
#include <cstddef>
#include <CubismFramework.hpp>

typedef void* csmDllModule;

class LibraryLoader
{
public:
    LibraryLoader()
        : module_(NULL)
    {}
    ~LibraryLoader()
    {
        Unload();
    }

    Csm::csmBool Load(const Csm::csmChar* dllPath);
    void Unload();
    template <typename T>
    T GetFuncPtr(const Csm::csmChar* funcName)
    {
        return reinterpret_cast<T>(GetFuncPtrInternal(funcName));
    }

    Csm::csmBool isValid() const
    {
        return module_;
    }

private:
    void* GetFuncPtrInternal(const Csm::csmChar* funcName);

    csmDllModule module_;
};

#endif // LIVE2D_0BA1D179C7E84EEE9FBFD714C7B87282_INCLUDED
