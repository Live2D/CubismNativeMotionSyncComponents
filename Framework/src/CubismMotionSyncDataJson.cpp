/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncDataJson.hpp"
#include <Math/CubismMath.hpp>

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

namespace {
const csmChar* Version = "Version";
const csmChar* Meta = "Meta";
const csmChar* SettingCount = "SettingCount";
const csmChar* Dictionary = "Dictionary";
const csmChar* Id = "Id";
const csmChar* Name = "Name";
const csmChar* Settings = "Settings";
const csmChar* AnalysisType = "AnalysisType";
const csmChar* UseCase = "UseCase";
const csmChar* CubismParameters = "CubismParameters";
const csmChar* Min = "Min";
const csmChar* Max = "Max";
const csmChar* Damper = "Damper";
const csmChar* Smooth = "Smooth";
const csmChar* AudioParameters = "AudioParameters";
const csmChar* Scale = "Scale";
const csmChar* Enabled = "Enabled";
const csmChar* Mappings = "Mappings";
const csmChar* Type = "Type";
const csmChar* Targets = "Targets";
const csmChar* Value = "Value";
const csmChar* PostProcessing = "PostProcessing";
const csmChar* BlendRatio = "BlendRatio";
const csmChar* Smoothing = "Smoothing";
const csmChar* SampleRate = "SampleRate";
}

CubismMotionSyncDataJson::CubismMotionSyncDataJson(const csmByte* buffer, csmSizeInt size)
{
    CreateCubismJson(buffer, size);
}

CubismMotionSyncDataJson::~CubismMotionSyncDataJson()
{
    DeleteCubismJson();
}

csmInt32 CubismMotionSyncDataJson::GetVersion() const
{
    return _json->GetRoot()[Version].ToInt();
}

CubismMotionSyncDataMeta CubismMotionSyncDataJson::GetMeta() const
{
    CubismMotionSyncDataMeta meta;

    meta.settingCount = _json->GetRoot()[Meta][SettingCount].ToInt();
    for (csmInt32 i = 0; i < meta.settingCount; i++)
    {
        CubismMotionSyncDataMetaDictionary dictionary;
        dictionary.id = _json->GetRoot()[Meta][Dictionary][i].GetString();
        dictionary.name = _json->GetRoot()[Meta][Dictionary][i].GetString();
        meta.dictionary.PushBack(dictionary);
    }

    return meta;
}

csmVector<CubismMotionSyncDataSetting> CubismMotionSyncDataJson::GetSettingList() const
{
    csmVector<CubismMotionSyncDataSetting> settingList;
    csmInt32 settingCount = _json->GetRoot()[Meta][SettingCount].ToInt();

    for (csmInt32 i = 0; i < settingCount; i++)
    {
        CubismMotionSyncDataSetting setting;

        // Id
        setting.id = _json->GetRoot()[Settings][i][Id].GetString();

        // Analysis type
        csmString analysisTypeStr = _json->GetRoot()[Settings][i][AnalysisType].GetString();
        if (analysisTypeStr == "CRI")
        {
            setting.analysisType = EngineType_Cri;
        }
        else
        {
            setting.analysisType = EngineType_Unknown;
        }

        // Set use case
        csmString useCaseStr = _json->GetRoot()[Settings][i][UseCase].GetString();
        if (useCaseStr == "Mouth")
        {
            setting.useCase = UseCase_Mouth;
        }
        else
        {
            setting.useCase = UseCase_Unknown;
        }

        // Set CubismParameter
        for (csmInt32 j = 0; j < _json->GetRoot()[Settings][i][CubismParameters].GetSize(); j++)
        {
            CubismMotionSyncDataCubismParameter cubismParameter;

            cubismParameter.name = _json->GetRoot()[Settings][i][CubismParameters][j][Name].GetString();
            cubismParameter.id = _json->GetRoot()[Settings][i][CubismParameters][j][Id].GetString();
            cubismParameter.min = _json->GetRoot()[Settings][i][CubismParameters][j][Min].ToFloat();
            cubismParameter.max = _json->GetRoot()[Settings][i][CubismParameters][j][Max].ToFloat();
            cubismParameter.damper = _json->GetRoot()[Settings][i][CubismParameters][j][Damper].ToFloat();
            cubismParameter.smooth = _json->GetRoot()[Settings][i][CubismParameters][j][Smooth].ToInt();

            setting.cubismParameterList.PushBack(cubismParameter);
        }

        // Set audio parameters
        for (csmInt32 j = 0; j < _json->GetRoot()[Settings][i][AudioParameters].GetSize(); j++)
        {
            CubismMotionSyncDataAudioParameter audioParameter;

            audioParameter.name = _json->GetRoot()[Settings][i][AudioParameters][j][Name].GetString();
            audioParameter.id = _json->GetRoot()[Settings][i][AudioParameters][j][Id].GetString();
            audioParameter.min = _json->GetRoot()[Settings][i][AudioParameters][j][Min].ToFloat();
            audioParameter.max = _json->GetRoot()[Settings][i][AudioParameters][j][Max].ToFloat();
            audioParameter.scale = _json->GetRoot()[Settings][i][AudioParameters][j][Scale].ToFloat();
            audioParameter.enabled = _json->GetRoot()[Settings][i][AudioParameters][j][Enabled].ToBoolean();

            setting.audioParameterList.PushBack(audioParameter);
        }

        // Set mappings
        for (csmInt32 j = 0; j < _json->GetRoot()[Settings][i][Mappings].GetSize(); j++)
        {
            CubismMotionSyncDataMapping mapping;

            csmString mappingTypeStr = _json->GetRoot()[Settings][i][Mappings][j][Type].GetString();
            if (mappingTypeStr == "Shape")
            {
                mapping.type = MappingType_Shape;
            }
            else
            {
                mapping.type = MappingType_Unknown;
            }
            mapping.id = _json->GetRoot()[Settings][i][Mappings][j][Id].GetString();
            for (csmInt32 k = 0; k < _json->GetRoot()[Settings][i][Mappings][j][Targets].GetSize(); k++)
            {
                CubismMotionSyncDataMappingTarget target;

                target.id = _json->GetRoot()[Settings][i][Mappings][j][Targets][k][Id].GetString();
                target.value = _json->GetRoot()[Settings][i][Mappings][j][Targets][k][Value].ToFloat();

                mapping.targetList.PushBack(target);
            }

            setting.mapping.PushBack(mapping);
        }

        // Set post processing
        setting.blendRatio = CubismMath::ClampF(_json->GetRoot()[Settings][i][PostProcessing][BlendRatio].ToFloat(), 0.0f, 1.0f);
        setting.smoothing = CubismMath::Clamp(_json->GetRoot()[Settings][i][PostProcessing][Smoothing].ToInt(), SMOOTHING_MIN_VALUE, SMOOTHING_MAX_VALUE);
        setting.sampleRate = CubismMath::ClampF(_json->GetRoot()[Settings][i][PostProcessing][SampleRate].ToFloat(), SAMPLE_RATE_MIN_VALUE, SAMPLE_RATE_MAX_VALUE);

        settingList.PushBack(setting);
    }

    return settingList;
}

}}}}
