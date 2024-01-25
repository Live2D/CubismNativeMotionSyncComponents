/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "CubismMotionSyncEngineAnalysisResult.hpp"
#include "Lib/CubismMotionSyncUtil.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class ICubismMotionSyncEngine;

class ICubismMotionSyncProcessor
{
public:
    /**
     * @brief 解析結果を格納するクラスを生成する
     *
     * @return 解析結果を格納するクラス
     */
    CubismMotionSyncEngineAnalysisResult* CreateAnalysisResult();

    /**
     * @brief モーションシンクの解析時に使用するマッピング情報のクラスを取得する
     *
     * @return モーションシンクの解析時に使用するマッピング情報のクラス
     */
    csmVector<CubismMotionSyncEngineMappingInfo> GetMappingInfoArray();

    /**
     * @brief プロセッサーのを作成したエンジンのタイプを取得
     *
     * @return プロセッサーのを作成したエンジンのタイプ
     */
    EngineType GetType();

    /**
     * @brief 解析で使用するサンプル数を取得する
     *
     * @return 解析で使用するサンプル数
     */
    csmUint32 GetRequireSampleCount();

    /**
     * @brief プロセッサーの状態（解放済みかどうか）を返す
     *
     * @return プロセッサーの状態
     */
    csmBool IsClosed();

    /**
     * @brief モーションシンクのコンテキストを解放する
     */
    void Close();

    /**
     * @brief モーションシンクのコンテキストのネイティブハンドルを取得する
     *
     * @return nativeHandle モーションシンクのコンテキストのネイティブハンドル
     */
    MotionSyncContext* GetContextHandle();

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ
     */
    ICubismMotionSyncProcessor(ICubismMotionSyncEngine *engine, MotionSyncContext *contextHandle, csmVector<CubismMotionSyncEngineMappingInfo> mappingInfoArray);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~ICubismMotionSyncProcessor();

protected:
    ICubismMotionSyncEngine* _engine;

private:
    csmVector<CubismMotionSyncEngineMappingInfo> _mappingInfoArray;
    MotionSyncContext *_contextHandle;
    };

}}}}
