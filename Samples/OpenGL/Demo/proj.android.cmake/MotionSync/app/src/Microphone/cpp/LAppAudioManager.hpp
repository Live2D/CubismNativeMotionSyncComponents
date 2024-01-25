/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "LAppMotionSyncDefine.hpp"
#include "CubismMotionSyncAudioBuffer.hpp"
#include "Type/csmString.hpp"
#include "Type/csmVector.hpp"

class LAppAudioManager
{
public:
    /**
     * @brief マイク入力の初期化
     *
     * @return マイク入力の初期化
     */
    Csm::csmBool SetupMicrophone(Csm::csmUint32 channels, Csm::csmUint32 samplesRate, Csm::csmUint32 bitDepth, Csm::csmUint32 useChannel);

    /**
     * @brief 音声の更新
     *
     * @return 更新結果
     */
    Csm::csmBool Update();

    /**
     * @brief 再生したバッファの格納先の設定
     *
     * @return バッファ
     */
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmFloat32>* GetBuffer();

    /**
     * @brief 解放処理
     *
     */
    void Release();

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ。
     *
     */
    LAppAudioManager();

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~LAppAudioManager();

private:
    // 録音用のID
    Csm::csmInt32 _audioInputId;
    // 再生用のID
    Csm::csmInt32 _audioOutputId;
    // 使用するチャンネル数
    Csm::csmInt32 _channels;
    // 使用するビット深度
    Csm::csmInt32 _bitDepth;
    // 使用するチャンネル
    Csm::csmInt32 _useChannel;
    // MotionSyncで使用するバッファ
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmFloat32> _buffer;
};
