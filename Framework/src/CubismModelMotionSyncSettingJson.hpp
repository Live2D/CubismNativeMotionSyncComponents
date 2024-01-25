/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "CubismModelSettingJson.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class CubismModelMotionSyncSettingJson : public CubismModelSettingJson
{
public:
    /**
     * @brief コンストラクタ
     *
     * コンストラクタ。
     *
     */
    CubismModelMotionSyncSettingJson(const csmByte* buffer, csmSizeInt size);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismModelMotionSyncSettingJson();

    /**
     * @brief motionsync3.jsonのファイル名を取得
     *
     * @return motionsync3.jsonのファイル名
     */
    const csmChar* GetMotionSyncJsonFileName();

    /**
     * @brief モーションシンクで使う音声データのファイルパスリストを取得
     *
     * @return 音声データのファイルパスリスト
     */
    csmVector<csmString> GetMotionSyncSoundFileList();

private:
    // MotionSync用のJSONノードのキャッシュ
    Utils::Value* _motionSyncJsonValue;

    /**
     * @brief motionsync3.jsonファイルがあるか返す
     *
     * @return motionsync3.jsonファイルがあるか
     */
    csmBool IsExistMotionSyncFiles() const;
};

}}}}
