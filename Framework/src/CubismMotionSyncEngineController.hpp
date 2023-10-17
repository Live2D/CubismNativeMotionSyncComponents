/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "Model/CubismModel.hpp"
#include "Id/CubismId.hpp"
#include "ICubismMotionSyncEngine.hpp"

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class CubismMotionSyncEngineController
{
public:
    /**
     * @brief 指定したファイルパスのDLLをロード
     *
     * @param[in]   dllFilePath    DLLファイルパス
     *
     * @return  
     */
    static ICubismMotionSyncEngine* InstallEngine(csmString dllFilePath);

    /**
     * @brief 指定された種類のエンジンを返す
     *
     * @param[in]   type    取得するエンジンの種類
     *
     * @return 指定した種類のエンジンインスタンス。見つからなければNULLを返す
     */
    static ICubismMotionSyncEngine* GetEngine(EngineType type);

    /**
     * @brief インストールされているエンジンをリストで返す
     * エンジンが1つもインストールされていない場合は空のリストを返します
     *
     * @return インストールされているエンジンのリスト
     */
    static csmVector<ICubismMotionSyncEngine*> GetEngines();

    /**
     * @brief エンジンを解放する
     *
     * @param[in]   engine    解放したいエンジン
     */
    static void ReleaseEngine(ICubismMotionSyncEngine *engine);

    /**
     * @brief エンジンを解放する
     *
     * @param[in]   engine    解放したいエンジン
     * @param[in]   isForce    強制的に解放するか
     */
    static void ReleaseEngine(ICubismMotionSyncEngine *engine, csmBool isForce);

    /**
     * 対象のエンジンとの関連を削除する。
     *
     * @param engine 対象のエンジン
     */
    static void DeleteAssociation(ICubismMotionSyncEngine *engine);

    /**
     * エンジンを全て削除する。
     *
     */
    static void DeleteAllEngine();

private:
    static csmMap<EngineType, ICubismMotionSyncEngine*> _engineMap;

    /**
     * @brief エンジン名からEngineTypeを求める 
     *
     * @param[in]   engineName    エンジン名
     *
     * @return EngineType
     */
    static EngineType ToEngineType(csmString engineName);

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ。
     */
    CubismMotionSyncEngineController();

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismMotionSyncEngineController();
};

}}}}
