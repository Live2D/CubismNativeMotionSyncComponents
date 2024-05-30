/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "ICubismMotionSyncEngine.hpp"
#include "CubismMotionSyncData.hpp"
#include "CubismMotionSyncEngineAnalysisResult.hpp"
#include "ICubismMotionSyncProcessor.hpp"
#include "CubismMotionSyncAudioBuffer.hpp"
#include "Type/csmVector.hpp"

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

/**
 * @brief モーションシンク機能
 *
 * モーションシンク機能を提供する。
 */
class CubismMotionSync
{
public:
    /**
     * @brief インスタンスの作成
     *
     * インスタンスを作成する。
     *
     * @param[in]   model  モデル情報
     * @param[in]   buffer  motionsync3.jsonが読み込まれているバッファ
     * @param[in]   size    バッファのサイズ
     * @param[in]   samplesPerSec    サンプリング周波数
     * @param[in]   executeAbsolutePath   実行中のプログラムファイルパス
     */
    static CubismMotionSync* Create(CubismModel *model, const csmByte *buffer, const csmSizeInt size, csmInt32 samplesPerSec, csmString executeAbsolutePath = "");

    /**
     * @brief インスタンスの破棄
     *
     * インスタンスを破棄する。
     *
     * @param[in]   instance    対象のCubismBreath
     */
    static void Delete(CubismMotionSync* instance);

    /**
     * @brief 音声バッファのポインタを設定する
     *
     * 音声バッファのポインタを設定する
     *
     * @param[in]   processIndex プロセッサーインデックス
     * @param[in]   buffer バッファ
     */
    void SetSoundBuffer(csmUint32 processIndex, CubismMotionSyncAudioBuffer<csmFloat32>* audioBuffer);

    /**
     * @brief モデルのパラメータの更新
     *
     * モデルのパラメータを更新する。
     *
     * @param[in]   model              対象のモデル
     * @param[in]   deltaTimeSeconds   デルタ時間[秒]
     */
    void UpdateParameters(CubismModel *model, csmFloat32 deltaTimeSeconds);

    /**
     * @brief プロセッサのblendRatioeを設定
     *
     * @param[in]   processIndex インデックス
     * @param[in]   blendRatio 設定する値
     */
    void SetBlendRatio(csmUint32 processIndex, csmFloat32 blendRatio);

    /**
     * @brief プロセッサのsmoothingを設定
     *
     * @param[in]   processIndex インデックス
     * @param[in]   smoothing 設定する値
     */
    void SetSmoothing(csmUint32 processIndex, csmInt32 smoothing);

    /**
     * @brief プロセッサのsampleRateを設定
     *
     * @param[in]   processIndex インデックス
     * @param[in]   sampleRate 設定する値
     */
    void SetSampleRate(csmUint32 processIndex, csmFloat32 sampleRate);

private:
    struct CubismProcessorInfo
    {
        /**
         * @brief コンストラクタ
         *
         * @param[in]   processor  プロセッサ
         * @param[in]   setting    jsonから読んだ中身
         *
         */
        CubismProcessorInfo(ICubismMotionSyncProcessor* processor, CubismModel* model, CubismMotionSyncDataSetting setting) :
            _processor(processor),
            _blendRatio(0.0f),
            _smoothing(1),
            _sampleRate(30.0f),
            _audioLevelEffectRatio(0.0f),
            _samplesAudioBuff(NULL),
            _model(model),
            _currentRemainTime(0.0f)
        {
            Init(setting);
            _analysisResult = _processor->CreateAnalysisResult();
        }

        /**
         * @brief デストラクタ
         */
        virtual ~CubismProcessorInfo()
        {}

        /**
         * @brief 値の初期化
         *
         * @param[in]   model    jsonから読んだ中身
         * @param[in]   setting    jsonから読んだ中身
         *
         */
        void Init(CubismMotionSyncDataSetting setting)
        {
            _currentRemainTime = 0.0f;
            for (csmUint32 i = 0; i < setting.cubismParameterList.GetSize(); i++)
            {
                _lastSmoothedList.PushBack(_model->GetParameterValue(setting.cubismParameterList[i].parameterIndex));
                _lastDampedList.PushBack(_model->GetParameterValue(setting.cubismParameterList[i].parameterIndex));
            }
            _blendRatio = setting.blendRatio;
            _smoothing = setting.smoothing;
            _sampleRate = setting.sampleRate;
        }

        ICubismMotionSyncProcessor *_processor;
        csmFloat32 _blendRatio;
        csmInt32 _smoothing;
        csmFloat32 _sampleRate;
        csmFloat32 _audioLevelEffectRatio;
        CubismMotionSyncAudioBuffer<csmFloat32>* _samplesAudioBuff;
        CubismModel *_model;
        CubismMotionSyncEngineAnalysisResult *_analysisResult;
        csmFloat32 _currentRemainTime;
        csmVector<csmFloat32> _lastSmoothedList;
        csmVector<csmFloat32> _lastDampedList;
    };

    CubismMotionSyncData *_data;
    csmVector<CubismProcessorInfo> _processorInfoList;

    /**
     * @brief jsonに書かれてるエンジン名からEngineTypeを求める
     *
     * @param[in]   engineName    エンジン名
     *
     * @return EngineType
     */
    static EngineType ToEngineType(csmString engineName);

    /**
     * @brief モデルのパラメータの更新
     *
     * モデルのパラメータを更新する。
     *
     * @param[in]   model        対象のモデル
     * @param[in]   processIndex インデックス
     */
    void Analyze(CubismModel *model, csmUint32 processIndex);

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ。
     *
     * @param[in]   model   モデルデータ
     * @param[in]   data    jsonから読み込んだデータ
     */
    CubismMotionSync(CubismModel* model, CubismMotionSyncData *data, csmVector<ICubismMotionSyncProcessor*> processorList);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismMotionSync();
};

}}}}
