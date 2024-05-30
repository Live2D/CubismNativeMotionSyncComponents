/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncData.hpp"
#include "CubismMotionSyncDataJson.hpp"

 //--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

CubismMotionSyncData* CubismMotionSyncData::Create(CubismModel* model, const csmByte* buffer, csmSizeInt size)
{
    CubismMotionSyncDataJson* json = CSM_NEW CubismMotionSyncDataJson(buffer, size);

    // motionsync3.jsonの読み取りに失敗したらNULLを返す
    if (!json->IsValid())
    {
        CSM_DELETE(json);
        return NULL;
    }

    csmInt32 version = json->GetVersion();
    CubismMotionSyncDataMeta meta = json->GetMeta();
    csmVector<CubismMotionSyncDataSetting> settingList = json->GetSettingList();

    CSM_DELETE(json);

    if (meta.dictionary.GetSize() == 0 || settingList.GetSize() == 0)
    {
        return NULL;
    }

    // パーツIDを特定しておく
    for (csmUint32 i = 0; i < settingList.GetSize(); i++)
    {
        for (csmUint32 j = 0; j < settingList[i].cubismParameterList.GetSize(); j++)
        {
            csmUint32 partIndex = model->GetParameterCount();

            for (csmUint32 k = 0; k < model->GetParameterCount(); k++)
            {
                if (model->GetParameterId(k)->GetString() == settingList[i].cubismParameterList[j].id)
                {
                    partIndex = k;
                    break;
                }
            }

            settingList[i].cubismParameterList[j].parameterIndex = partIndex;
        }
    }

    return CSM_NEW CubismMotionSyncData(version, meta, settingList);
}

void CubismMotionSyncData::Delete(CubismMotionSyncData* motionSyncData)
{
    CSM_DELETE_SELF(CubismMotionSyncData, motionSyncData);
}

csmInt32 CubismMotionSyncData::GetVersion()
{
    return _version;
}

CubismMotionSyncDataMeta CubismMotionSyncData::GetMeta()
{
    return _meta;
}

CubismMotionSyncDataSetting CubismMotionSyncData::GetSetting(const csmUint32 index)
{
   if (_settingList.GetSize() <= index)
   {
       return _settingList[0];
   }

   return _settingList[index];
}

csmVector<CubismMotionSyncEngineMappingInfo> CubismMotionSyncData::GetMappingInfo(const csmInt32 index)
{
    csmVector<CubismMotionSyncEngineMappingInfo> mappingInfoList;

    if (_settingList.GetSize() <= index)
    {
        return mappingInfoList;
    }

    CubismMotionSyncDataSetting setting = _settingList[index];

    for (csmUint32 j = 0; j < setting.audioParameterList.GetSize(); j++)
    {
        // AudioParameterId
        csmString audioParameterId = setting.audioParameterList[j].id;

        // ModelParameterIds & ModelParameterValues
        csmVector<csmString> modelParameterIds;
        csmVector<csmFloat32> modelParameterValues;
        // IDが一致するか確認する
        for (csmUint32 searchPos = 0; searchPos < setting.audioParameterList.GetSize(); searchPos++)
        {
            if (audioParameterId == setting.mapping[searchPos].id)
            {
                for (csmUint32 k = 0; k < setting.cubismParameterList.GetSize(); k++)
                {
                    // ModelParameterIds
                    modelParameterIds.PushBack(setting.mapping[searchPos].targetList[k].id);

                    // ModelParameterValues
                    modelParameterValues.PushBack(setting.mapping[searchPos].targetList[k].value);
                }
                break;
            }
        }

        // Scale
        csmFloat32 scale = setting.audioParameterList[j].scale;

        // Enabled
        csmBool enabled = setting.audioParameterList[j].enabled;

        mappingInfoList.PushBack(CubismMotionSyncEngineMappingInfo(audioParameterId, modelParameterIds, modelParameterValues, scale, enabled));
    }

    return mappingInfoList;
}

CubismMotionSyncData::CubismMotionSyncData(csmInt32 version, CubismMotionSyncDataMeta meta, csmVector<CubismMotionSyncDataSetting> settingList) :
    _version(version),
    _meta(meta),
    _settingList(settingList)
{
}

CubismMotionSyncData::~CubismMotionSyncData()
{
}

}}}}
