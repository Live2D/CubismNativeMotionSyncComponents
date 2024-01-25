/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <dsound.h>
#include "LAppMotionSyncDefine.hpp"
#include "CubismMotionSyncAudioBuffer.hpp"
#include "Type/csmString.hpp"
#include "Type/csmVector.hpp"

class LAppMicrophoneAudioManager
{
public:
    /**
     * @brief 音声機能を使うための初期化
     *
     * @param[in]   data    音声機能を使うために必要なデータ
     *
     * @return 初期化結果
     */
    static Csm::csmBool Init(HWND window, Csm::csmInt32 channels, Csm::csmInt32 samplesPerSec, Csm::csmInt32 bitDepth);

    /**
     * @brief 音声機能を終了
     *
     * @return 終了結果
     */
    static Csm::csmBool Close();

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
    LAppMicrophoneAudioManager();

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~LAppMicrophoneAudioManager();

private:
    LPDIRECTSOUNDCAPTURE8 _soundCapture;
    LPDIRECTSOUNDCAPTUREBUFFER _captureBuffer;
    LPDIRECTSOUNDBUFFER _secondary;
    // 使用するチャンネル数
    Csm::csmInt32 _channels;
    // 使用するビット深度
    Csm::csmInt32 _bitDepth;
    // 音声再生用のバッファの大きさ
    Csm::csmUint32 _bufferSampleBytes;
    // 使用するチャンネル
    Csm::csmInt32 _useChannel;
    // 録音時の読み込み位置
    DWORD _inputPos;
    // 再生時の書き込み位置
    DWORD _outputPos;
    // MotionSyncで使用するバッファ
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmFloat32> _buffer;
};
