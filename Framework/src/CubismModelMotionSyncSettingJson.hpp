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

    /**
    * @brief モーションシンク設定の名称を .model3.json の Motion から取得する。
    */
    const csmChar* GetMotionSyncSettingNameFromMotion(const csmChar* groupName, csmInt32 index) const;

    /**
     * @brief モーションシンクで使うモーションファイルと音声ファイルの結びつけを行うマップを取得
     */
    void GetMotionSyncLinkAudioMap(csmMap<csmString, csmString>& fileMap);

    /**
     * @brief モーションシンクで使うモーションファイルパスとモーションシンク設定の結びつけを行うマップを取得
     */
    void GetMotionSyncLinkSettingMap(csmMap<csmString, csmString>& fileMap);

private:
    // MotionSync用のJSONノードのキャッシュ
    Utils::Value* _motionSyncJsonValue;

    /**
     * @brief motionsync3.jsonファイルがあるか返す
     *
     * @return motionsync3.jsonファイルがあるか
     */
    csmBool IsExistMotionSyncFiles() const;

    /**
     * @brief Motionキーにモーションシンク設定名があるか返す
     *
     * @return モーションシンク設定名があるか
     */
    csmBool IsExistMotionSyncSettingNameFromMotion(const csmChar* groupName, csmInt32 index) const;
};

}}}}
