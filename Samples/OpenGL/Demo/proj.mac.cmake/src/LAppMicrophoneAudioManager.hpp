/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <AudioToolbox/AudioQueue.h>
#include "LAppMotionSyncDefine.hpp"
#include "CubismMotionSyncAudioBuffer.hpp"
#include "Type/csmString.hpp"
#include "Type/csmVector.hpp"
#include "LAppMutex.hpp"

class LAppMicrophoneAudioManager
{
public: 
    /**
     * @brief マイク入力の初期化
     *
     * @return 初期化が成功したか
     */
    Csm::csmBool SetupMicrophone(Csm::csmUint32 channels, Csm::csmUint32 samplesRate, Csm::csmUint32 bitDepth, Csm::csmUint32 useChannel);

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
    AudioQueueRef _inputQueue;
    AudioQueueRef _outputQueue;
    Csm::csmUint32 _queueBufferSize;
    Csm::csmUint32 _queueBufferSampleCount;
    // 録音したデータ
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmVector<Csm::csmFloat32>> _inputDataList;
    // 使用するチャンネル
    Csm::csmUint32 _useChannel;
    // 使用するチャンネル数
    Csm::csmInt32 _channels;
    // MotionSyncで使用するバッファ
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmFloat32> _buffer;
    // mutex
    LAppMutex _mutex;
    
    /**
     * @brief 録音データをバッファに格納する
     *
     * @param[in]   samples 録音データ
     */
    void WriteInputBuffer(Csm::csmFloat32* samples);
    
    /**
     * @brief 録音データをバッファから取り出す
     *
     * @param[in]   samples 録音データの取り出し先
     */
    void ReadInputBuffer(Csm::csmFloat32* samples);
    
    /**
     * @brief 録音時のコールバック処理
     *
     */
    static void InputCallBack(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer, const AudioTimeStamp *startTime, UInt32 packetNum, const AudioStreamPacketDescription *packetDesc);
    
    /**
     * @brief 再生時コールバック処理
     *
     */
    static void OutputCallBack(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer);
};
