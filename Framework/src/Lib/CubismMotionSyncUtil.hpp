/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "ICubismMotionSyncEngineLib.hpp"
#include "CubismMotionSyncEngineMappingInfo.hpp"

 //--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

enum EngineType
{
    EngineType_Cri,
    EngineType_Unknown,
};

class MappingInfoListMapper
{
public:
    // ----------- コンストラクタ・デストラクタ -----------
    MappingInfoListMapper()
    {}

    ~MappingInfoListMapper()
    {
        DeleteMappingInfoList();
    }

// ----------- メンバ関数 -----------
public:
    void SetJObject(csmVector<CubismMotionSyncEngineMappingInfo> infoList)
    {
        DeleteMappingInfoList();
        ConvertObjectToNative(infoList);
    }

    const CubismMotionSyncMappingInfo* Data()
    {
        return _infoList.GetPtr();
    }

    csmUint32 Count() const
    {
        return _infoList.GetSize();
    }

private:
    void ConvertObjectToNative(csmVector<CubismMotionSyncEngineMappingInfo> mapInfoList)
    {
        const csmInt32 infoListCount = mapInfoList.GetSize();


        for (csmInt32 infoIdx = 0; infoIdx < infoListCount; infoIdx++)
        {
            CubismMotionSyncEngineMappingInfo& mapInfo = mapInfoList[infoIdx];
            CubismMotionSyncMappingInfo info;
            // Enabled
            info.Enabled = mapInfo.IsEnabled();
            // Scale
            info.Scale = mapInfo.GetScale();
            // AudioParameterId
            csmChar* buff = static_cast<csmChar*>(CSM_MALLOC(sizeof(csmChar) * (mapInfo.GetAudioParameterId().GetLength() + 1)));
            strcpy(buff, mapInfo.GetAudioParameterId().GetRawString());
            info.AudioParameterId = buff;
            // ModelParameterValues
            info.ModelParameterValues = static_cast<csmFloat32*>(CSM_MALLOC(sizeof(csmFloat32) * mapInfo.GetModelParameterValues().GetSize()));
            for (csmUint32 i = 0; i < mapInfo.GetModelParameterValues().GetSize(); i++)
            {
                info.ModelParameterValues[i] = mapInfo.GetModelParameterValues()[i];
            }
            // ModelParameterIds
            info.ModelParameterCount = mapInfo.GetModelParameterIds().GetSize();
            csmChar** buffs = static_cast<csmChar**>(CSM_MALLOC(sizeof(csmChar*) * mapInfo.GetModelParameterIds().GetSize()));
            for (csmUint32 i = 0; i < mapInfo.GetModelParameterIds().GetSize(); i++)
            {
                buffs[i] = static_cast<csmChar*>(CSM_MALLOC(sizeof(csmChar) * (mapInfo.GetModelParameterIds()[i].GetLength() + 1)));
                strcpy(buffs[i], mapInfo.GetModelParameterIds()[i].GetRawString());
            }
            info.ModelParameterIds = const_cast<const csmChar**>(buffs);

            _infoList.PushBack(info);
        }
    }

    void DeleteMappingInfoList()
    {
        for (csmUint32 infoIdx = 0; infoIdx < _infoList.GetSize(); ++infoIdx)
        {
            const CubismMotionSyncMappingInfo& info = _infoList[infoIdx];

            CSM_FREE(const_cast<csmChar*>(info.AudioParameterId));
            CSM_FREE(info.ModelParameterValues);

            for (csmUint32 paramIdx = 0; paramIdx < info.ModelParameterCount; ++paramIdx)
            {
                CSM_FREE(const_cast<csmChar*>(info.ModelParameterIds[paramIdx]));
            }
            CSM_FREE(info.ModelParameterIds);
        }

        _infoList.Clear();
    }

// ----------- メンバ変数 -----------
private:
    csmVector<CubismMotionSyncMappingInfo> _infoList;
};

struct MotionSyncContext
{
public:
    MotionSyncContext(CubismMotionSyncContext context, MappingInfoListMapper* mapper)
        : _context(context), _mapper(mapper)
    {
    }

    ~MotionSyncContext()
    {
        CSM_DELETE(_mapper);
        _mapper = NULL;
        _context = NULL;
    }

    CubismMotionSyncContext Context() const
    {
        return _context;
    }

    MappingInfoListMapper* Mapper() const
    {
        return _mapper;
    }

private:
    CubismMotionSyncContext _context;
    MappingInfoListMapper* _mapper;
};

}}}}
