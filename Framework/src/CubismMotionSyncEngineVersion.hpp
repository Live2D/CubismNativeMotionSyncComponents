/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "Type/csmString.hpp"
#include "Type/CubismBasicType.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

/**
 * Cubism MotionSync Engineのバージョン情報を扱うクラス。
 * フォーマットは xx.yy.zzzz となる。
 */
class CubismMotionSyncEngineVersion
{
public:
    /**
     * @brief コンストラクタ
     *
     * コンストラクタ。
     *
     * @param[in]   version  エンジンから取得した生のバージョン情報
     */
    CubismMotionSyncEngineVersion(csmInt32 version);

    /**
     * @brief   Cubism MotionSync Engineのメジャーバージョンを返す。
     *
     * @return      メジャーバージョン
     */
    csmInt32 GetMajor();

    /**
     * @brief   Cubism MotionSync Engineのマイナーバージョンを返す。
     *
     * @return      マイナーバージョン
     */
    csmInt32 GetMinor();

    /**
     * @brief   Cubism MotionSync Engineのパッチ番号を返す。
     *
     * @return      パッチ番号
     */
    csmInt32 GetPatch();

    /**
     * @brief   Cubism MotionSync Engineのバージョン番号を返す。
     *
     * @return      バージョン番号
     */
    csmInt32 GetVersionNumber();

    /**
     * @brief   Cubism MotionSync Engineのバージョン情報を返す。
     *
     * @return      バージョン情報
     */
    csmString ToString();

private:
    csmInt32 _versionNumber;
    csmInt32 _major;
    csmInt32 _minor;
    csmInt32 _patch;
};

}}}}
