/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "Type/csmVector.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

class CubismMotionSyncEngineMappingInfo
{
public:
    /**
     * @brief マッピング情報の口形素IDを文字列で返す
     *
     * @return マッピング情報の口形素ID
     */
    csmString GetAudioParameterId();

    /**
     * @brief モデルのパラメータIDの配列を返す
     *
     * @return パラメータIDの配列
     */
    csmVector<csmString> GetModelParameterIds();

    /**
     * @brief モデルのパラメータ値の配列を返す
     *
     * @return パラメータ値の配列
     */
    csmVector<csmFloat32> GetModelParameterValues();

    /**
     * @brief スケール値を返す
     *
     * @return スケール値
     */
    csmFloat32 GetScale();

    /**
     * @brief このマッピング情報が有効かどうかを返す
     *
     * @return マッピング情報の有効・無効。有効ならtrue
     */
    csmBool IsEnabled();

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ
     *
     * @param audioParameterId     口形素ID
     * @param modelParameterIds    モデルのパラメータIDの配列
     * @param modelParameterValues モデルのパラメータ値
     * @param scale                倍率(0.1~10.0)
     * @param enabled              マッピング情報の有効・無効
     */
    CubismMotionSyncEngineMappingInfo(csmString audioParameterId, csmVector<csmString> modelParameterIds, csmVector<csmFloat32> modelParameterValues, csmFloat32 scale, csmBool enabled);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismMotionSyncEngineMappingInfo();

private:
    csmString _audioParameterId;
    csmVector<csmString> _modelParameterIds;
    csmVector<csmFloat32> _modelParameterValues;
    csmFloat32 _scale;
    csmBool _enabled;
};

}}}}
