/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */
#pragma once

#include <CubismFramework.hpp>

/**
* @brief  Sample Appで使用する定数
*
*/
namespace LAppMotionSyncDefine {

    using namespace Csm;

    extern const csmUint32 DirectSoundBufferSampleCount; ///< DirectSount の再生バッファに設定されるサンプル数

    extern const csmChar* FastForwardImageName; ///< 早送りファイル

    // 音声設定
    extern const csmInt32 Channels; ///< チャンネル数
    extern const csmInt32 SamplesPerSec; ///< サンプリング周波数
    extern const csmInt32 BitDepth; ///< ビット深度
}
