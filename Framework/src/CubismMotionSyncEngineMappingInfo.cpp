/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSyncEngineMappingInfo.hpp"

 //--------- LIVE2D NAMESPACE ------------
namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

csmString CubismMotionSyncEngineMappingInfo::GetAudioParameterId()
{
    return _audioParameterId;
}

csmVector<csmString> CubismMotionSyncEngineMappingInfo::GetModelParameterIds()
{
    return _modelParameterIds;
}

csmVector<csmFloat32> CubismMotionSyncEngineMappingInfo::GetModelParameterValues()
{
    return _modelParameterValues;
}

csmFloat32 CubismMotionSyncEngineMappingInfo::GetScale()
{
    return _scale;
}

csmBool CubismMotionSyncEngineMappingInfo::IsEnabled()
{
    return _enabled;
}

CubismMotionSyncEngineMappingInfo::CubismMotionSyncEngineMappingInfo(csmString audioParameterId, csmVector<csmString> modelParameterIds, csmVector<csmFloat32> modelParameterValues, csmFloat32 scale, csmBool enabled) :
    _audioParameterId(audioParameterId),
    _modelParameterIds(modelParameterIds),
    _modelParameterValues(modelParameterValues),
    _scale(scale),
    _enabled(enabled)
{
    if (audioParameterId.GetLength() == 0)
    {
        CubismLogError("The audio parameter ID is null.");
    }
    if (modelParameterIds.GetSize() != modelParameterValues.GetSize())
    {
        CubismLogError("The array length of IDs differs from the array length of parameter values. Please make them the same.");
    }
    if (modelParameterIds.GetSize() == 0)
    {
        CubismLogError("The model parameter ID array or the model parameter value array length is 0.");
    }
    if (!(0.1 <= scale && scale <= 10.0))
    {
        CubismLogError("The value of scale is incorrect. The value is limited to between 0.1 and 10.0.");
    }
}

CubismMotionSyncEngineMappingInfo::~CubismMotionSyncEngineMappingInfo()
{
}

}}}}
