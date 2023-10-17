/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncEngineVersion.hpp"
#include <cstdio>

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

CubismMotionSyncEngineVersion::CubismMotionSyncEngineVersion(csmInt32 version) :
    _versionNumber(version),
    _major((version >> 24) & 0xFF),
    _minor((version >> 16) & 0xFF),
    _patch(version & 0xFF)
{
}

csmInt32 CubismMotionSyncEngineVersion::GetMajor()
{
    return _major;
}

csmInt32 CubismMotionSyncEngineVersion::GetMinor()
{
    return _minor;
}

csmInt32 CubismMotionSyncEngineVersion::GetPatch()
{
    return _patch;
}

csmInt32 CubismMotionSyncEngineVersion::GetVersionNumber()
{
    return _versionNumber;
}

csmString CubismMotionSyncEngineVersion::ToString()
{
    csmChar buff[64];
    sprintf(buff, "%02d.%02d.%04d (%d)", _major, _minor, _patch, _versionNumber);
    return  buff;
}

}}}}
