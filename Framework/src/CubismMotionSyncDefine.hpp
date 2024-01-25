/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#ifdef _WIN32
#define MOTIONSYNC_CORE_CRI_LIB_NAME "Live2DCubismMotionSyncEngine_CRI.dll"
#elif __MACH__
#define MOTIONSYNC_CORE_CRI_LIB_NAME "libLive2DCubismMotionSyncEngine_CRI.dylib"
#elif __ANDROID__
#define MOTIONSYNC_CORE_CRI_LIB_NAME "libLive2DCubismMotionSyncEngine_CRI.so"
#else
#define MOTIONSYNC_CORE_CRI_LIB_NAME 
#endif

#define MOTIONSYNC_CORE_CRI_LIB_FILE MOTIONSYNC_CORE_CRI_LIB_PATH MOTIONSYNC_CORE_CRI_LIB_NAME

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {


}}}}
