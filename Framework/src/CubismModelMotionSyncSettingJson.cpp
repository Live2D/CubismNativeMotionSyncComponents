/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismModelMotionSyncSettingJson.hpp"

//--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

namespace {
const csmChar* FileReferences = "FileReferences";
const csmChar* MotionSync = "MotionSync";
}

CubismModelMotionSyncSettingJson::CubismModelMotionSyncSettingJson(const csmByte* buffer, csmSizeInt size) :
    CubismModelSettingJson(buffer, size)
{
    if (_json)
    {
        _motionSyncJsonValue = &_json->GetRoot()[FileReferences][MotionSync];
    }
}

CubismModelMotionSyncSettingJson::~CubismModelMotionSyncSettingJson()
{
}

const csmChar* CubismModelMotionSyncSettingJson::GetMotionSyncJsonFileName()
{
    if (!IsExistMotionSyncFiles())
    {
        return "";
    }
    return _json->GetRoot()[FileReferences][MotionSync].GetRawString();
}

csmVector<csmString> CubismModelMotionSyncSettingJson::GetMotionSyncSoundFileList()
{
    csmVector<csmString> list;
    const csmChar* groupName;
    
    for (csmUint32 i = 0; i < GetMotionGroupCount(); i++)
    {
        groupName = GetMotionGroupName(i);

        for (csmUint32 j = 0; j < GetMotionCount(groupName); j++)
        {
            const csmString fileName = GetMotionSoundFileName(groupName, j);

            if (fileName.GetLength() > 0)
            {
                list.PushBack(fileName);
            }
        }
    }

    return list;
}

void CubismModelMotionSyncSettingJson::GetMotionSyncLinkAudioMap(csmMap<csmString, csmString>& fileMap)
{
    const csmChar* groupName;

    // モーショングループのリストをまわす
    for (csmInt32 i = 0; i < GetMotionGroupCount(); i++)
    {
        groupName = GetMotionGroupName(i);

        // モーションのリストをまわす
        for (csmInt32 motionIndex = 0; motionIndex < GetMotionCount(groupName); motionIndex++)
        {
            // 音声ファイルの名称取得
            const csmString audioFileName = GetMotionSoundFileName(groupName, motionIndex);

            // キーが存在したら
            if (audioFileName.GetLength() > 0)
            {
                // モーションファイル名をキーにマップへ登録
                fileMap[GetMotionFileName(groupName, motionIndex)] = GetMotionSoundFileName(groupName, motionIndex);
            }
        }
    }
}

const csmChar* CubismModelMotionSyncSettingJson::GetMotionSyncSettingNameFromMotion(const csmChar* groupName, csmInt32 index) const
{
    // キーが存在するかをチェックして存在しなければ空を返す
    if (!IsExistMotionGroupName(groupName))
    {
        return "";
    }
    return (*_jsonValue[FrequentNode_Motions])[groupName][index][MotionSync].GetRawString();
}

void CubismModelMotionSyncSettingJson::GetMotionSyncLinkSettingMap(csmMap<csmString, csmString>& fileMap)
{
    const csmChar* groupName;

    // モーショングループのリストをまわす
    for (csmInt32 i = 0; i < GetMotionGroupCount(); i++)
    {
        groupName = GetMotionGroupName(i);

        // モーションのリストをまわす
        for (csmInt32 motionIndex = 0; motionIndex < GetMotionCount(groupName); motionIndex++)
        {
            // モーションシンク設定の名称取得
            const csmString settingName = GetMotionSyncSettingNameFromMotion(groupName, motionIndex);

            // キーが存在したら
            if (settingName.GetLength() > 0)
            {
                // モーションファイル名をキーにマップへ登録
                fileMap[GetMotionFileName(groupName, motionIndex)] = settingName;
            }
        }
    }
}

csmBool CubismModelMotionSyncSettingJson::IsExistMotionSyncFiles() const
{
    Utils::Value& node = *_motionSyncJsonValue;
    return !node.IsNull() && !node.IsError();
}
}}}}
