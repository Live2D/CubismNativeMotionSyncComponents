/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "ICubismMotionSyncEngineLib.hpp"

class CubismMotionSyncEngineLoader;

class CubismMotionSyncEngineLib : public ICubismMotionSyncEngineLib
{
public:
    CubismMotionSyncEngineLib();
    ~CubismMotionSyncEngineLib();

    Csm::csmBool LoadLibrary(const Csm::csmChar* dllPath);
    void UnloadLibrary();

    /**
    * Queries the version of Engine.
    *
    * @return  Version of Engine.
    */
    CubismMotionSyncVersion GetEngineVersion();

    /**
    * Queries the name of Engine.
    *
    * @return  Name of Engine.
    */
    const Csm::csmChar* GetEngineName();

    /**
    * Initializes the Engine.

    * @param engineConfig Congifuration parameter for engine initialization. Set to `NULL` to use default parameter.
    *
    * @return `true` if Engine is available.
    */
    Csm::csmBool InitializeEngine(const void* engineConfig);

    /**
    * Disposes the Engine.
    */
    void DisposeEngine();

    /**
    * Makes the context of Analysis.
    *
    * @param contextConfig Congifuration parameter for engine initialization. Set to `NULL` to use default parameter.
    * @param mappingInfoList Array of Mapping Information of AudioParameter.
    * @param mappingInfoListCount Length of `mappingInfoList`.
    *
    * @return Valid pointer on success; '0' otherwise.
    */
    CubismMotionSyncContext CreateContext(
        const void* contextConfig
        , const CubismMotionSyncMappingInfo* mappingInfoList
        , Csm::csmInt32 mappingInfoListCount
    );

    /**
    * Resets the context of Analysis for reusing.
    *
    * @param context Context to reset.
    */
    void ClearContext(CubismMotionSyncContext context);

    /**
    * Destroys the context of Analysis.
    *
    * @param context Context to destroy.
    */
    void DeleteContext(CubismMotionSyncContext context);

    /**
    * Gets the number of samples to analyze at a time.
    *
    * @param context Context to query.
    *
    * @return Number of samples.
    */
    Csm::csmUint32 GetRequireSampleCount(CubismMotionSyncContext context);

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
    Csm::csmBool Analyze(
        CubismMotionSyncContext context
        , const Csm::csmFloat32* samples
        , Csm::csmUint32 sampleCount
        , CubismMotionSyncAnalysisResult* analysisResult
        , const void* analyzeConfig
    );

private:
    CubismMotionSyncEngineLoader* _engineIf;
};
