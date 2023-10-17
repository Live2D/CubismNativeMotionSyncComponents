/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LibraryLoader.h"
#include <windows.h>

#define DYNAMIC_LIBRARY_LOAD(X) ::LoadLibraryW(X)
#define DYNAMIC_LIBRARY_INSTANCE ::HMODULE
#define DYNAMIC_LIBRARY_FREE ::FreeLibrary
#define DYNAMIC_LIBRARY_LOAD_PROC ::GetProcAddress

 // UTF8→UTF16文字列を変換する
 // 変換された文字列は、delete[] で解放する必要がある。
static wchar_t* ConvertUtf8ToUtf16(const Csm::csmChar* utf8String)
{
    // 変換後のwchar_t配列の要素数(null終端を含む)を取得
    const Csm::csmInt32 size = ::MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, NULL, 0);
    wchar_t* utf16String = static_cast<wchar_t*>(CSM_MALLOC(sizeof(wchar_t) * size));
    ::MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, utf16String, size);

    return utf16String;
}

Csm::csmBool LibraryLoader::Load(const Csm::csmChar* dllPath)
{
    //DLLロード済みの場合
    if (isValid())
    {
        return true;
    }

    wchar_t* u16DllPath = ConvertUtf8ToUtf16(dllPath);
    module_ = DYNAMIC_LIBRARY_LOAD(u16DllPath);
    CSM_FREE(u16DllPath);

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

void* LibraryLoader::GetFuncPtrInternal(const Csm::csmChar* funcName)
{
    if (!isValid())
    {
        return NULL;
    }
    return DYNAMIC_LIBRARY_LOAD_PROC(reinterpret_cast<DYNAMIC_LIBRARY_INSTANCE>(module_), funcName);
}
