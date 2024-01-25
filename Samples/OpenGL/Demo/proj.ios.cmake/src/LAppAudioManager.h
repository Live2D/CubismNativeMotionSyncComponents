/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#import <AudioToolbox/AudioQueue.h>
#import "LAppMotionSyncDefine.h"
#import "CubismMotionSyncAudioBuffer.hpp"
#import "Type/csmString.hpp"
#import "Type/csmVector.hpp"

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
    AudioQueueRef _queue;
    Csm::csmUint32 _queueBufferSize;
    Csm::csmUint32 _queueBufferSampleCount;
    Csm::csmVector<Csm::csmFloat32> _wavSamples;
    Csm::csmUint32 _wavWritePos;
    Csm::csmUint32 _useChannel;
    Csm::csmInt32 _channels;
    Csm::csmInt32 _bitDepth;
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmFloat32> _buffer;
    
    /**
     * @brief コールバック処理
     *
     */
    static void CallBack(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer);
    
};
