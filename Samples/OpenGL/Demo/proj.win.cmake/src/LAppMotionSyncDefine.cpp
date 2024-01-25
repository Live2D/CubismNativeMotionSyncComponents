/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppMotionSyncDefine.hpp"

namespace LAppMotionSyncDefine {

    using namespace Csm;

    // DirectSount の再生バッファに設定されるサンプル数(音声ファイル再生時)
    const csmUint32 DirectSoundBufferSoundFileSampleCount = 8192;
    // DirectSount の再生バッファに設定されるサンプル数(マイク入力時)
    const csmUint32 DirectSoundBufferMicrophoneSampleCount = 73728; // 65536 + 8192
    // リングバッファの大きさ
    const csmUint32 CsmRingBufferSize = 4096;

    // 早送り
    const csmChar* FastForwardImageName = "icon_fastForward.png";

    // チャンネル数
    const csmInt32 Channels = 2; 
    // サンプリング周波数
    const csmInt32 SamplesPerSec = 48000;
    // ビット深度
    const csmInt32 BitDepth = 16;
}
