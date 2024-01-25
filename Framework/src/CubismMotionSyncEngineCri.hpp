/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "CubismMotionSyncProcessorCri.hpp"
#include "ICubismMotionSyncEngine.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class CubismMotionSyncEngineCri : public ICubismMotionSyncEngine
{
public:
    /**
     * @brief 音声解析のためのプロセッサーを作成する。
     *
     * @param mappingInfoArray マッピング情報の配列(length >= 6)
     * @param sampleRate サンプリングレート(16000~128000)
     * @return プロセッサーのインスタンス
     */
    CubismMotionSyncProcessorCri* CreateProcessor(csmVector<CubismMotionSyncEngineMappingInfo> mappingInfoArray, csmInt32 sampleRate);

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ
     *
     * @param[in]   engineHandle    エンジンのハンドル
     * @param[in]   type            エンジンの種類
     * @param[in]   name            エンジンの名前
     * @param[in]   version         エンジンのバージョン
     */
    CubismMotionSyncEngineCri(ICubismMotionSyncEngineLib* engineHandle, EngineType type, csmString name, CubismMotionSyncEngineVersion version);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismMotionSyncEngineCri();
};

}}}}
