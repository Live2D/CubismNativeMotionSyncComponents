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
			const csmChar* fileName = GetMotionSoundFileName(groupName, j);

			if (strlen((fileName)))
			{
				list.PushBack(fileName);
			}
		}
	}

	return list;
}

csmBool CubismModelMotionSyncSettingJson::IsExistMotionSyncFiles() const
{
	Utils::Value& node = *_motionSyncJsonValue;
	return !node.IsNull() && !node.IsError();
}
}}}}
