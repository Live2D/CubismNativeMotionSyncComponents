/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "CubismJsonHolder.hpp"
#include "CubismMotionSyncData.hpp"
#include "CubismMotionSyncEngineMappingInfo.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class CubismMotionSyncDataJson : public CubismJsonHolder
{
public:
    /**
     * @brief コンストラクタ
     *
     * コンストラクタ。
     *
     * @param[in]   buffer  motionsync3.jsonが読み込まれているバッファ
     * @param[in]   size    バッファのサイズ
     */
    CubismMotionSyncDataJson(const csmByte* buffer, csmSizeInt size);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismMotionSyncDataJson();

    /**
      * @brief バージョンを取得
      *
      * motionsync3.jsonのバージョンを取得
      *
      * @return  バージョン
      */
    csmInt32 GetVersion() const;

    /**
     * @brief Meta情報を取得
     *
     * motionsync3.jsonのMeta情報を取得
     *
     * @return  Meta情報
     */
    CubismMotionSyncDataMeta GetMeta() const;

    /**
     * @brief Settings情報を取得
     *
     * motionsync3.jsonのSettings情報を取得
     *
     * @return  Settings情報
     */
    csmVector<CubismMotionSyncDataSetting> GetSettingList() const;
};

}}}}
