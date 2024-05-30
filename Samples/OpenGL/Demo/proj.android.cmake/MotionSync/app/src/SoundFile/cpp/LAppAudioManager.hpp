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
    Csm::csmInt32 _audioId;
    // 正規化される前の音声データ
    Csm::csmByte* _data;
    // 正規化される前の音声データの大きさ
    Csm::csmUint64 _dataSize;
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
