﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppMotionSyncDefine.hpp"

namespace LAppMotionSyncDefine {

    using namespace Csm;

    // DirectSount の再生バッファに設定されるサンプル数
    const csmUint32 DirectSoundBufferSampleCount = 8192; 

    // 早送り
    const csmChar* FastForwardImageName = "icon_fastForward.png";

    // チャンネル数
    const csmInt32 Channels = 2; 
    // サンプリング周波数
    const csmInt32 SamplesPerSec = 48000;
    // ビット深度
    const csmInt32 BitDepth = 16;
}
