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

class LAppAudioManager
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
     * @brief 音声ファイル読み込み
     *
     * @param[in]   path    音声ファイル
     * @param[in]   useChannel    使用するチャンネル
     *
     * @return 読み込み結果
     */
    Csm::csmBool LoadFile(Csm::csmString path, Csm::csmUint32 useChannel);

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
    enum WritePosition
    {
        WritePosition_Front,
        WritePosition_Back,
    };

    LPDIRECTSOUNDBUFFER _secondary;
    // 使用するチャンネル数
    Csm::csmInt32 _channels;
    // 使用するビット深度
    Csm::csmInt32 _bitDepth;
    // 音声再生書き込み位置
    WritePosition _writePosition;
    // 音声再生用のバッファの大きさ
    Csm::csmUint32 _bufferSampleBytes;
    // 正規化される前の音声データ
    Csm::csmByte* _data;
    // 正規化される前の音声データの大きさ
    Csm::csmUint64 _dataSize;
    // 正規化される前の音声データの書き込み位置
    Csm::csmUint64 _dataPos;
    // 正規化後の音声データ
    Csm::csmFloat32* _samples;
    // 正規化後の音声データの大きさ
    Csm::csmUint32 _samplesSize;
    // 正規化後の音声データの書き込み位置
    Csm::csmUint32 _samplesPos;
    // MotionSyncで使用するバッファ
    Csm::MotionSync::CubismMotionSyncAudioBuffer<Csm::csmFloat32> _buffer;
    // 音声ファイル読み込み済か
    Csm::csmBool _isLoadFile;
};
