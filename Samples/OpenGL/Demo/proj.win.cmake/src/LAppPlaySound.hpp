/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <dsound.h>
#include "LAppMotionSyncDefine.hpp"
#include "Type/csmString.hpp"
#include "Type/csmVector.hpp"

class LAppPlaySound
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
    Csm::csmVector<Csm::csmFloat32>* GetBuffer();

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
    LAppPlaySound();

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~LAppPlaySound();

private:
    enum WritePosition
    {
        WritePosition_Front,
        WritePosition_Back,
    };

    LPDIRECTSOUNDBUFFER _secondary;
    Csm::csmInt32 _channels;
    Csm::csmInt32 _bitDepth;
    WritePosition _writePosition;
    Csm::csmUint32 _bufferSampleBytes;
    Csm::csmByte* _data;
    Csm::csmUint64 _dataSize;
    Csm::csmUint64 _dataPos;
    Csm::csmFloat32* _samples;
    Csm::csmUint32 _samplesSize;
    Csm::csmVector<Csm::csmFloat32> _buffer;
};
