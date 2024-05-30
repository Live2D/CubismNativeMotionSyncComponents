/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "CubismMotionSyncEngineVersion.hpp"
#include "Lib/ICubismMotionSyncEngineLib.hpp"
#include "Lib/CubismMotionSyncUtil.hpp"
#include "Type/csmVector.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class ICubismMotionSyncProcessor;

class ICubismMotionSyncEngine
{
public:
    /**
     * @brief モーションシンクエンジンの種別を返す
     *
     * @return name モーションシンクエンジンの種別
     */
    EngineType GetType();

    /**
     * @brief モーションシンクエンジンの名前を返す
     *
     * @return エンジン名
     */
    csmString GetName();

    /**
     * @brief モーションシンクエンジンのバージョンを返す
     *
     * @return version バージョン
     */
    CubismMotionSyncEngineVersion GetVersion();

    /**
     * @brief モーションシンクエンジンが解放されているかをチェックする
     *
     * @return version バージョン
     */
    csmBool IsClosed();

    /**
     * @brief プロセッサーのリストを返す
     *
     * @return プロセッサーのリスト
     */
    csmVector<ICubismMotionSyncProcessor*> GetProcessors();

    /**
     * @brief モーションシンクエンジンが持つプロセッサーをすべて解放する
     */
    void ReleaseAllProcessor();

    /**
     * @brief モーションシンクエンジンのネイティブ側のハンドルを返す
     *
     * @return モーションシンクエンジンのネイティブハンドル
     */
    ICubismMotionSyncEngineLib* GetEngineHandle();

    /**
     * @brief モーションシンクエンジンを解放する。
     *
     * @param isForce 解放フラグ
     */
    void Close(csmBool isForce);

    /**
     * @brief 対象のプロセッサーとエンジンとの関連を削除する
     *
     * @param processor 対象のプロセッサー
     */
    void DeleteAssociation(ICubismMotionSyncProcessor* processor);

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
    ICubismMotionSyncEngine(ICubismMotionSyncEngineLib *engineHandle, EngineType type, csmString name, CubismMotionSyncEngineVersion version);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~ICubismMotionSyncEngine();

protected:
    csmVector<ICubismMotionSyncProcessor*> _processors;

private:
    ICubismMotionSyncEngineLib *_engineHandle;
    EngineType _type;
    csmString _name;
    CubismMotionSyncEngineVersion _version;
};

}}}}
