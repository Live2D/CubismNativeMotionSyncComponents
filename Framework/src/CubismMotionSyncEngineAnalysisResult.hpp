/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "Type/CubismBasicType.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class CubismMotionSyncEngineAnalysisResult
{
public:
    /**
     * @brief 解析された値の配列を取得する
     *
     * @return values 値の配列
     */
    csmFloat32* GetValues();

    /**
     * @brief 解析された値の配列の大きさを取得する
     *
     * @return values 値の配列
     */
    csmInt32 GetValuesCount();

    /**
     * @brief 解析されたサンプルの数を設定する
     *
     */
    void SetProcessedSampleCount(csmInt32 sampleCount);

    /**
     * @brief 解析されたサンプルの数を取得する
     *
     * @return processedSampleCount 解析されたサンプルの数
     */
    csmInt32 GetProcessedSampleCount();

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ
     */
    CubismMotionSyncEngineAnalysisResult(csmInt32 valuesSize);

    /**
     * @brief コピーコンストラクタ
     *
     * コピーコンストラクタ
     */
    CubismMotionSyncEngineAnalysisResult(const CubismMotionSyncEngineAnalysisResult &result);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismMotionSyncEngineAnalysisResult();

private:
    csmFloat32* _values;
    csmInt32 _valuesCount;
    csmInt32 _processedSampleCount;
};

}}}}
