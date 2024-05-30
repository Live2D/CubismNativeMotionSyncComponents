/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "CubismMotionSync.hpp"
#include "CubismMotionSyncDataJson.hpp"
#include "CubismMotionSyncEngineController.hpp"
#include "CubismMotionSyncEngineCri.hpp"
#include "CubismMotionSyncDefine.hpp"
#include "Math/CubismMath.hpp"

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

CubismMotionSync* CubismMotionSync::Create(CubismModel *model, const csmByte *buffer, const csmSizeInt size, csmInt32 samplesPerSec, csmString executeAbsolutePath)
{
    csmString enginePath;

    if (!buffer)
    {
        CubismLogError("Buffer is null in CubismMotionSync::Create().");
        return NULL;
    }

    CubismMotionSyncData *data = CubismMotionSyncData::Create(model, buffer, size);

    if (!data)
    {
        return NULL;
    }

    // エンジンインストールとプロセッサ作成
    csmVector<ICubismMotionSyncProcessor*> processorList;
    for (csmInt32 i = 0; i < data->GetMeta().settingCount; i++)
    {
        ICubismMotionSyncEngine *engine = NULL;
        ICubismMotionSyncProcessor *processor = NULL;
        EngineType engineType = data->GetSetting(i).analysisType;

        switch(engineType)
        {
        case EngineType_Cri:
            enginePath = executeAbsolutePath + MOTIONSYNC_CORE_CRI_LIB_FILE;
            engine = CubismMotionSyncEngineController::GetEngine(engineType);
            if (!engine)
            {
                engine = CubismMotionSyncEngineController::InstallEngine(enginePath);
            }
            if (engine)
            {
                processor = static_cast<CubismMotionSyncEngineCri*>(engine)->CreateProcessor(data->GetMappingInfo(i), samplesPerSec);
            }
            else
            {
                CubismLogError("Failed Install Engine : %s", enginePath.GetRawString());
            }
            break;
        default:
            break;
        }
        if (processor)
        {
            processorList.PushBack(processor);
        }
    }

    CubismMotionSync* motionSync = CSM_NEW CubismMotionSync(model, data, processorList);

    return motionSync;
}

void CubismMotionSync::Delete(CubismMotionSync* instance)
{
    CSM_DELETE_SELF(CubismMotionSync, instance);
}

void CubismMotionSync::SetSoundBuffer(csmUint32 processIndex, CubismMotionSyncAudioBuffer<csmFloat32>* audioBuffer)
{
    if (processIndex < _processorInfoList.GetSize())
    {
        _processorInfoList[processIndex]._samplesAudioBuff = audioBuffer;
    }
}

void CubismMotionSync::UpdateParameters(CubismModel *model, csmFloat32 deltaTimeSeconds)
{
    // 設定から時間を変更すると、経過時間がマイナスになることがあるので、経過時間0として対応。
    if (deltaTimeSeconds < 0.0f)
    {
        deltaTimeSeconds = 0.0f;
    }

    for (csmUint32 processIndex = 0; processIndex < _processorInfoList.GetSize(); processIndex++)
    {
        _processorInfoList[processIndex]._currentRemainTime += deltaTimeSeconds;

        // Check each time assuming it may have been updated.
        csmFloat32 fps = _processorInfoList[processIndex]._sampleRate;

        csmFloat32 processorDeltaTime = 1.0f / fps;

        // If the specified frame time is not reached, no analysis is performed.
        if (_processorInfoList[processIndex]._currentRemainTime < processorDeltaTime)
        {
            for (csmUint32 targetIndex = 0; targetIndex < _data->GetSetting(processIndex).cubismParameterList.GetSize(); targetIndex++)
            {
                if (isnan(_processorInfoList[processIndex]._analysisResult->GetValues()[targetIndex]))
                {
                    continue;
                }

                // Overwrite parameter values every frame to prevent data from replacing itself
                model->SetParameterValue(_data->GetSetting(processIndex).cubismParameterList[targetIndex].parameterIndex, _processorInfoList[processIndex]._lastDampedList[targetIndex]);
            }
            continue;
        }

        Analyze(model, processIndex);

        // Reset counter.
        _processorInfoList[processIndex]._currentRemainTime = CubismMath::ModF(_processorInfoList[processIndex]._currentRemainTime, processorDeltaTime);

        for (csmUint32 targetIndex = 0; targetIndex < _data->GetSetting(processIndex).cubismParameterList.GetSize(); targetIndex++)
        {
            if (isnan(_processorInfoList[processIndex]._analysisResult->GetValues()[targetIndex]))
            {
                continue;
            }

            // Overwrite parameter values every frame to prevent data from replacing itself
            model->SetParameterValue(_data->GetSetting(processIndex).cubismParameterList[targetIndex].parameterIndex, _processorInfoList[processIndex]._lastDampedList[targetIndex]);
        }
    }
}

void CubismMotionSync::SetBlendRatio(csmUint32 processIndex, csmFloat32 blendRatio)
{
    if (processIndex < _processorInfoList.GetSize())
    {
        _processorInfoList[processIndex]._blendRatio = blendRatio;
    }
}

void CubismMotionSync::SetSmoothing(csmUint32 processIndex, csmInt32 smoothing)
{
    if (processIndex < _processorInfoList.GetSize())
    {
        _processorInfoList[processIndex]._smoothing = smoothing;
    }
}

void CubismMotionSync::SetSampleRate(csmUint32 processIndex, csmFloat32 sampleRate)
{
    if (processIndex < _processorInfoList.GetSize())
    {
        _processorInfoList[processIndex]._sampleRate = sampleRate;
    }
}

EngineType CubismMotionSync::ToEngineType(csmString engineName)
{
    EngineType engineType = EngineType_Unknown;

    if (engineName == "CRI")
    {
        engineType = EngineType_Cri;
    }

    return engineType;
}

void CubismMotionSync::Analyze(CubismModel* model, csmUint32 processIndex)
{
    ICubismMotionSyncProcessor* processor = _processorInfoList[processIndex]._processor;
    CubismMotionSyncAudioBuffer<csmFloat32>* samples = _processorInfoList[processIndex]._samplesAudioBuff;

    if (!processor || !samples)
    {
        return;
    }

    CubismMotionSyncEngineAnalysisResult* analysis = NULL;

    csmFloat32 blendRatio = _processorInfoList[processIndex]._blendRatio;
    csmInt32 smoothing = _processorInfoList[processIndex]._smoothing;
    csmFloat32 audioLevelEffectRatio = _processorInfoList[processIndex]._audioLevelEffectRatio;

    const csmUint32 sampleSize = samples->GetSize();
    csmUint32 requireSampleCount = processor->GetRequireSampleCount();

    for (csmUint32 i = requireSampleCount; i < sampleSize; i += requireSampleCount)
    {
        switch (processor->GetType())
        {
        case EngineType_Cri:
            analysis = static_cast<CubismMotionSyncProcessorCri*>(processor)->Analyze(samples, blendRatio, smoothing, audioLevelEffectRatio, _processorInfoList[processIndex]._analysisResult);
            break;
        default:
            break;
        }

        if (!analysis)
        {
            break;
        }

        samples->Remove(analysis->GetProcessedSampleCount());

        // モーションシンクライブラリで計算した内容をモデルに繁栄
        for (csmUint32 targetIndex = 0; targetIndex < _data->GetSetting(processIndex).cubismParameterList.GetSize(); targetIndex++)
        {
            csmFloat32 cacheValue = analysis->GetValues()[targetIndex];

            if (isnan(cacheValue))
            {
                continue;
            }

            const csmFloat32 smooth = static_cast<csmFloat32>(_data->GetSetting(processIndex).cubismParameterList[targetIndex].smooth);
            const csmFloat32 damper = _data->GetSetting(processIndex).cubismParameterList[targetIndex].damper;

            // Smoothing
            cacheValue = ((100.0f - smooth) * cacheValue + _processorInfoList[processIndex]._lastSmoothedList[targetIndex] * smooth) / 100.0f;

            // Assign value after smoothing
            _processorInfoList[processIndex]._lastSmoothedList[targetIndex] = cacheValue;

            // Dampening
            if (CubismMath::AbsF(cacheValue - _processorInfoList[processIndex]._lastDampedList[targetIndex]) < damper)
            {
                cacheValue = _processorInfoList[processIndex]._lastDampedList[targetIndex];
            }

            // Assign value after dampening.
            _processorInfoList[processIndex]._lastDampedList[targetIndex] = cacheValue;
        }

        requireSampleCount = processor->GetRequireSampleCount();
    }
}

CubismMotionSync::CubismMotionSync(CubismModel* model, CubismMotionSyncData *data, csmVector<ICubismMotionSyncProcessor*> processorList) :
    _data(data)
{
    for (csmUint32 i = 0; i < processorList.GetSize(); i++)
    {
        _processorInfoList.PushBack(CubismProcessorInfo(processorList[i], model, data->GetSetting(i)));
        _processorInfoList[i].Init(data->GetSetting(i));
    }
}

CubismMotionSync::~CubismMotionSync()
{
    CubismMotionSyncData::Delete(_data);
    for (csmUint32 i = 0; i < _processorInfoList.GetSize(); i++)
    {
        if (_processorInfoList[i]._processor)
        {
            _processorInfoList[i]._processor->Close();
        }
    }
}

}}}}
