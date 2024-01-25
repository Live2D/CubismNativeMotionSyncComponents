/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <cstddef>
#include <CubismFramework.hpp>

/************************************************************************
 *               Cと互換性のある構造体として定義
 * Live2DCubismMotionSyncEngine.h と同じ構造体として定義すること。
 ************************************************************************/
 /** Context of Analysis. */
typedef void* CubismMotionSyncContext;

/** Engine version identifier. */
typedef Csm::csmUint32 CubismMotionSyncVersion;

/**
 * Result of analysis.
 */
struct CubismMotionSyncAnalysisResult
{
    CubismMotionSyncAnalysisResult(
        Csm::csmFloat32* values = NULL
        , Csm::csmInt32 valuesCount = NULL
        , Csm::csmUint32 processedSampleCount = 0)
        : Values(values),
        ValuesCount(valuesCount),
        ProcessedSampleCount(processedSampleCount) {}

    /** Value Array of CubismParameter */
    Csm::csmFloat32* Values;

    /** Number of values used in the analysis. */
    Csm::csmInt32 ValuesCount;

    /** Number of samples used in the analysis. */
    Csm::csmUint32 ProcessedSampleCount;
};

/** Mapping Information of AudioParameter. */
struct CubismMotionSyncMappingInfo
{
    CubismMotionSyncMappingInfo(
        const Csm::csmChar* audioParameterId = NULL
        , const Csm::csmChar** modelParameterIds = NULL
        , Csm::csmFloat32* modelParameterValues = NULL
        , Csm::csmUint32 modelParameterCount = 0
        , Csm::csmFloat32 scale = 0.0f
        , Csm::csmUint32 enabled = 0)
        : AudioParameterId(audioParameterId),
        ModelParameterIds(modelParameterIds),
        ModelParameterValues(modelParameterValues),
        ModelParameterCount(modelParameterCount),
        Scale(scale),
        Enabled(enabled) {}
    ~CubismMotionSyncMappingInfo()
    {
    }

    /** ID of AudioParameter. */
    const Csm::csmChar* AudioParameterId;

    /** Array of ID of ModelParameter linked to the AudioParameter. */
    const Csm::csmChar** ModelParameterIds;

    /** Array of ModelParameter Value. */
    Csm::csmFloat32* ModelParameterValues;

    /** Number of ModelParameter. */
    Csm::csmUint32 ModelParameterCount;

    /** Scaling value for CubismParameter. */
    Csm::csmFloat32 Scale;

    /** Enable / Disable for applying.*/
    Csm::csmUint32 Enabled;
};

class ICubismMotionSyncEngineLib
{
public:
    ICubismMotionSyncEngineLib() {};
    virtual ~ICubismMotionSyncEngineLib() {};

    virtual Csm::csmBool LoadLibrary(const Csm::csmChar* dllPath) { return true; };
    virtual void UnloadLibrary() {};

    /**
    * Queries the version of Engine.
    *
    * @return  Version of Engine.
    */
    virtual CubismMotionSyncVersion GetEngineVersion() = 0;

    /**
    * Queries the name of Engine.
    *
    * @return  Name of Engine.
    */
    virtual const Csm::csmChar* GetEngineName() = 0;

    /**
    * Initializes the Engine.

    * @param engineConfig Congifuration parameter for engine initialization. Set to `NULL` to use default parameter.
    *
    * @return `true` if Engine is available.
    */
    virtual Csm::csmBool InitializeEngine(const void* engineConfig) = 0;

    /**
    * Disposes the Engine.
    */
    virtual void DisposeEngine() = 0;

    /**
    * Makes the context of Analysis.
    *
    * @param contextConfig Congifuration parameter for engine initialization. Set to `NULL` to use default parameter.
    * @param mappingInfoList Array of Mapping Information of AudioParameter.
    * @param mappingInfoListCount Length of `mappingInfoList`.
    *
    * @return Valid pointer on success; '0' otherwise.
    */
    virtual CubismMotionSyncContext CreateContext(
        const void* contextConfig
        , const CubismMotionSyncMappingInfo* mappingInfoList
        , Csm::csmInt32 mappingInfoListCount
    ) = 0;

    /**
    * Resets the context of Analysis for reusing.
    *
    * @param context Context to reset.
    */
    virtual void ClearContext(CubismMotionSyncContext context) = 0;

    /**
    * Destroys the context of Analysis.
    *
    * @param context Context to destroy.
    */
    virtual void DeleteContext(CubismMotionSyncContext context) = 0;

    /**
    * Gets the number of samples to analyze at a time.
    *
    * @param context Context to query.
    *
    * @return Number of samples.
    */
    virtual Csm::csmUint32 GetRequireSampleCount(CubismMotionSyncContext context) = 0;

    /**
    * Analyzes the samples for conversion to the CubismParameter.
    *
    * @param context Context to analyze.
    * @param samples Array of sample to analyze.
    * @param sampleCount Number of samples.
    * @param analysisResult Array to store results.
    *
    * @return `true` if succeeded the analysis.
    */
    virtual Csm::csmBool Analyze(
        CubismMotionSyncContext context
        , const Csm::csmFloat32* samples
        , Csm::csmUint32 sampleCount
        , CubismMotionSyncAnalysisResult* analysisResult
        , const void* analyzeConfig
    ) = 0;
};
