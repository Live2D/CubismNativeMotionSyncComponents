/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "CubismMotionSyncEngineMappingInfo.hpp"
#include "ICubismMotionSyncEngine.hpp"
#include "Model/CubismUserModel.hpp"
#include "Type/csmVector.hpp"

#define SMOOTHING_MIN_VALUE 1
#define SMOOTHING_MAX_VALUE 100
#define SAMPLE_RATE_MIN_VALUE 1.0f
#define SAMPLE_RATE_MAX_VALUE 120.0f

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

struct CubismMotionSyncDataMetaDictionary
{
    csmString id;
    csmString name;
};

struct CubismMotionSyncDataMeta
{
    csmInt32 settingCount;
    csmVector<CubismMotionSyncDataMetaDictionary> dictionary;
};

enum CubismMotionSyncDataUseCase
{
    UseCase_Mouth,
    UseCase_Unknown,
};

struct CubismMotionSyncDataCubismParameter
{
    csmString name;
    csmString id;
    csmFloat32 min;
    csmFloat32 max;
    csmFloat32 damper;
    csmInt32 smooth;

    csmUint32 parameterIndex;
};

struct CubismMotionSyncDataAudioParameter
{
    csmString name;
    csmString id;
    csmFloat32 min;
    csmFloat32 max;
    csmFloat32 scale;
    csmBool enabled;
};

enum CubismMotionSyncDataMappingType
{
    MappingType_Shape,
    MappingType_Unknown,
};

struct CubismMotionSyncDataMappingTarget
{
    csmString id;
    csmFloat32 value;
};

struct CubismMotionSyncDataMapping
{
    CubismMotionSyncDataMappingType type;
    csmString id;
    csmVector<CubismMotionSyncDataMappingTarget> targetList;
};

struct CubismMotionSyncDataSetting
{
public:
    csmString id;
    EngineType analysisType;
    CubismMotionSyncDataUseCase useCase;
    csmVector<CubismMotionSyncDataCubismParameter> cubismParameterList;
    csmVector<CubismMotionSyncDataAudioParameter> audioParameterList;
    csmVector<CubismMotionSyncDataMapping> mapping;
    csmFloat32 blendRatio;
    csmInt32 smoothing;
    csmFloat32 sampleRate;
};

class CubismMotionSyncData
{
public:

    /**
     * @brief インスタンスの作成
     *
     * インスタンスを作成する。
     *
     * @param[in]   buffer      userdata3.jsonが読み込まれいるバッファ
     * @param[in]   size        バッファのサイズ
     * @return      作成されたインスタンス
     */
    static CubismMotionSyncData* Create(CubismModel* model, const csmByte* buffer, csmSizeInt size);

    /**
     * @brief インスタンスの破棄
     *
     * インスタンスを破棄する。
     *
     * @param[in]   motionSyncData      破棄するインスタンス
     */
    static void Delete(CubismMotionSyncData*motionSyncData);

    /**
      * @brief バージョンを取得
      *
      * バージョンを取得
      *
      * @return  バージョン
      */
    csmInt32 GetVersion();

    /**
     * @brief metaを取得
     *
     * metaを取得
     *
     * @return  meta
     */
    CubismMotionSyncDataMeta GetMeta();

    /**
     * @brief settingを取得
     *
     * settingを取得
     *
     * @param[in]   index      取得したい位置
     *
     * @return  setting
     */
    CubismMotionSyncDataSetting GetSetting(const csmUint32 index);

    /**
     * @brief MappingInfoを取得
     *
     * MappingInfoを取得
     *
     * @param[in]   index      取得したい位置
     *
     * @return  MappingInfo
     */
    csmVector<CubismMotionSyncEngineMappingInfo> GetMappingInfo(const csmInt32 index);

    /**
     * @brief デストラクタ
     *
     * デストラクタ。
     */
    virtual ~CubismMotionSyncData();

private:
    csmInt32 _version;
    CubismMotionSyncDataMeta _meta;
    csmVector<CubismMotionSyncDataSetting> _settingList;

    /**
     * @brief コンストラクタ
     *
     * コンストラクタ。
     *
     * @param[in]   version  motionsync3.jsonのバージョン
     * @param[in]   meta  motionsync3.jsonに含まれているmeta
     * @param[in]   settings  motionsync3.jsonに含まれているsettings
     */
    CubismMotionSyncData(csmInt32 version, CubismMotionSyncDataMeta meta, csmVector<CubismMotionSyncDataSetting> settingList);
};

}}}}
