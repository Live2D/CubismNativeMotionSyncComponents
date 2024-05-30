/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "ICubismMotionSyncEngine.hpp"
#include "ICubismMotionSyncProcessor.hpp"
#include "CubismMotionSyncAudioBuffer.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class CubismMotionSyncProcessorCri : public ICubismMotionSyncProcessor
{
public:
    /**
     * @brief このプロセッサーが保持するサンプルレートの値を返す。
     *
     * @return サンプルレートの値
     */
    csmInt32 GetSampleRate();

    /**
     * @brief このプロセッサーが保持する音声のビットデプスの値を返す。
     *
     * @return 音声のビットデプス
     */
    csmInt32 GetBitDepth();

    /**
     * @brief 音声データから口形素を解析する。
     * 解析結果のインスタンスをメソッド内で作成する。
     *
     * @param samples               解析したい音声データ
     * @param blendRatio            ブレンド率(0.0~1.0)
     * @param smoothing             スムージングの値(1~100)
     * @param audioLevelEffectRatio 音量の大きさが口の開閉に与える影響度(0.0~1.0)
     * @param result                解析結果のインスタンス
     *
     * @return 解析結果のインスタンス
     */
    CubismMotionSyncEngineAnalysisResult* Analyze(CubismMotionSyncAudioBuffer<csmFloat32>* samples, csmFloat32 blendRatio, csmInt32 smoothing, csmFloat32 audioLevelEffectRatio, CubismMotionSyncEngineAnalysisResult* result);

    /**
     * @brief 音声データから口形素を解析する
     * 解析結果のインスタンスをメソッド内で作成する
     * NOTE: メモリ効率を考慮する必要がある場合には、解析結果のインスタンスを引数に取る方のメソッドを使用すること
     *
     * @param samples               解析したい音声データ
     * @param blendRatio            ブレンド率(0.0f~1.0f)
     * @param smoothing             スムージングの値(1~100)
     * @param audioLevelEffectRatio 音量の大きさが口の開閉に与える影響度(0.0~1.0)  ※現在未使用。互換性のため残される
     *
     * @return 解析結果のインスタンス
     */
    CubismMotionSyncEngineAnalysisResult* Analyze(CubismMotionSyncAudioBuffer<csmFloat32>* samples, csmFloat32 blendRatio, csmInt32 smoothing, csmFloat32 audioLevelEffectRatio);

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ
     *
     * @param engine           モーションシンクエンジンのインスタンス
     * @param nativeHandle     ネイティブハンドル
     * @param mappingInfoArray マッピング情報の配列
     * @param sampleRate       サンプリングレート(16000~128000で設定すること)
     * @param bitDepth         音声のビットデプス(16,32bit)
     */
    CubismMotionSyncProcessorCri(ICubismMotionSyncEngine *engine, MotionSyncContext *contextHandle, csmVector<CubismMotionSyncEngineMappingInfo> mappingInfoArray, csmInt32 sampleRate, csmInt32 bitDepth);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismMotionSyncProcessorCri();

private:
    csmInt32 _sampleRate;
    csmInt32 _bitDepth;
};

}}}}
