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

class LAppAudioManager
{
public:
    /**
     * @brief 音声ファイル読み込み
     *
     * @param[in]   path    音声ファイル
     * @param[in]   useChannel    使用するチャンネル
     *
     * @return 読み込み結果
     */
    Csm::csmBool LoadFile(Csm::csmString path, Csm::csmUint32 useChannel);

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
     * @brief 再生中か確認
     *
     * @return 再生中か
     */
    Csm::csmBool IsPlay();

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
    /**
     * @brief 音声ファイル再生用のコールバック処理
     *
     */
    static void CallBackForAudioFile(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer);
       
    /**
     * @brief マイク入力時の録音時コールバック処理
     *
     */
    static void InputCallBackForMicrophone(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer, const AudioTimeStamp *startTime, UInt32 packetNum, const AudioStreamPacketDescription *packetDesc);
    
    /**
     * @brief マイク入力時の再生時コールバック処理
     *
     */
    static void OutputCallBackForMicrophone(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer);

    /**
     * @brief マイク入力時の録音データをバッファに格納する
     *
     * @param[in]   samples 録音データ
     */
    void WriteInputBuffer(Csm::csmFloat32* samples);
    
    /**
     * @brief マイク入力時の録音データをバッファから取り出す
     *
     * @param[in]   samples 録音データの取り出し先
     */
    void ReadInputBuffer(Csm::csmFloat32* samples);

    AudioQueueRef _inputQueue;
    AudioQueueRef _outputQueue;
    Csm::csmUint32 _queueBufferSize;
    Csm::csmUint32 _queueBufferSampleCount;
    // 録音したデータ
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmVector<Csm::csmFloat32>> _inputDataList;
    // wavデータ
    Csm::csmVector<Csm::csmFloat32> _wavSamples;
    // wavデータ書き込み位置
    Csm::csmUint32 _wavWritePos;
    // 使用するチャンネル
    Csm::csmUint32 _useChannel;
    // 使用するチャンネル数
    Csm::csmInt32 _channels;
    // 使用するビット深度
    Csm::csmInt32 _bitDepth;
    // MotionSyncで使用するバッファ
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmFloat32> _buffer;
    // 音声ファイル読み込み済か
    Csm::csmBool _isLoadFile;
    // mutex
    LAppMutex _mutex;
};
