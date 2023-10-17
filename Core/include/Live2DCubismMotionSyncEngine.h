/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LIVE2D_CUBISM_MOTIONSYNC_ENGINE_H
#define LIVE2D_CUBISM_MOTIONSYNC_ENGINE_H

#if defined(__cplusplus)
extern "C"
{
#endif

    /* ------- *
     * DEFINES *
     * ------- */

     /** MotionSync API attribute. */
#if !defined(csmMotionSyncApi)
#define csmMotionSyncApi
#endif

    /* ----- *
     * TYPES *
     * ----- */

     /** Context of Analysis. */
    typedef void* csmMotionSync_Context;

    /** Engine version identifier. */
    typedef unsigned int csmMotionSyncVersion;


    /** Booleans. */
    enum
    {
        csmMotionSyncFalse = 0,
        csmMotionSyncTrue = 1
    };

    /**
    * Result of analysis.
    */
    typedef struct
    {
        /** Value Array of CubismParameter */
        float* Values;

        /** Number of values used in the analysis. */
        int ValuesCount;

        /** Number of samples used in the analysis. */
        unsigned int ProcessedSampleCount;
    } csmMotionSync_AnalysisResult;

    /** Mapping Information of AudioParameter. */
    typedef struct
    {
        /** ID of AudioParameter. */
        const char* AudioParameterId;

        /** Array of ID of ModelParameter linked to the AudioParameter. */
        const char** ModelParameterIds;

        /** Array of ModelParameter Value. */
        float* ModelParameterValues;

        /** Number of ModelParameter. */
        unsigned int ModelParameterCount;

        /** Scaling value for CubismParameter. */
        float Scale;

        /** Enable / Disable for applying.*/
        unsigned int Enabled;
    } csmMotionSync_MappingInfo;

    /** Log handler.
     *
     * @param  message  Null-terminated string message to log.
     */
    typedef void (*csmMotionSyncLogFunction)(const char* message);

    /** Calling conventions for each platform. */
#if CSM_MOTIONSYNC_ENGINE_WIN32_DLL
#define csmMotionSyncCallingConvention __stdcall
#else
#define csmMotionSyncCallingConvention
#endif

    /* ------- *
     * VERSION *
     * ------- */

     /**
      * Queries the version of Engine.
      *
      * @return  Version of Engine.
      */
    csmMotionSyncApi csmMotionSyncVersion csmMotionSyncCallingConvention csmMotionSync_GetEngineVersion();

    /**
     * Queries the name of Engine.
     *
     * @return  Name of Engine.
     */
    csmMotionSyncApi const char* csmMotionSyncCallingConvention csmMotionSync_GetEngineName();

    /* ------- *
     * LOGGING *
     * ------- */

     /**
      * Queries log handler.
      *
      * @return  Log handler.
      */
    csmMotionSyncApi csmMotionSyncLogFunction csmMotionSyncCallingConvention csmMotionSync_GetLogFunction();

    /**
     * Sets log handler.
     *
     * @param  handler  Handler to use.
     */
    csmMotionSyncApi void csmMotionSyncCallingConvention csmMotionSync_SetLogFunction(csmMotionSyncLogFunction handler);

    /* ------- *
     * METHOD *
     * ------- */

     /**
      * Initializes the Engine.

      * @param engineConfiguration Congifuration parameter for engine initialization. Set to `NULL` to use default parameter.
      *
      * @return `1` if Engine is available.
      */
    csmMotionSyncApi int csmMotionSyncCallingConvention csmMotionSync_InitializeEngine(const void* engineConfiguration);

    /**
     * Disposes the Engine.
     */
    csmMotionSyncApi void csmMotionSyncCallingConvention csmMotionSync_DisposeEngine();

    /**
     * Makes the context of Analysis.
     *
     * @param contextConfiguration Congifuration parameter for engine initialization. Set to `NULL` to use default parameter.
     * @param mappingInformations Array of Mapping Information of AudioParameter.
     * @param mappingInformationCount Length of `mappingInformations`.
     *
     * @return Valid pointer on success; '0' otherwise.
     */
    csmMotionSyncApi csmMotionSync_Context csmMotionSyncCallingConvention csmMotionSync_CreateContext(const void* contextConfiguration, const csmMotionSync_MappingInfo* mappingInformations, const int mappingInformationCount);

    /**
     * Resets the context of Analysis for reusing.
     *
     * @param context Context to reset.
     */
    csmMotionSyncApi void csmMotionSyncCallingConvention csmMotionSync_ClearContext(const csmMotionSync_Context context);

    /**
     * Destroys the context of Analysis.
     *
     * @param context Context to destroy.
     */
    csmMotionSyncApi void csmMotionSyncCallingConvention csmMotionSync_DeleteContext(csmMotionSync_Context context);

    /**
     * Gets the number of samples to analyze at a time.
     *
     * @param context Context to query.
     *
     * @return Number of samples.
     */
    csmMotionSyncApi unsigned int csmMotionSyncCallingConvention csmMotionSync_GetRequireSampleCount(const csmMotionSync_Context context);

    /**
     * Analyzes the samples for conversion to the CubismParameter.
     *
     * @param context Context to analyze.
     * @param samples Array of sample to analyze.
     * @param sampleCount Number of samples.
     * @param analysisResult Array to store results.
     * @param analysisConfiguration Configuration of analysis.
     *
     * @return `1` if succeeded the analysis.
     */
    csmMotionSyncApi int csmMotionSyncCallingConvention csmMotionSync_Analyze(const csmMotionSync_Context context, const float* samples, const unsigned int sampleCount, csmMotionSync_AnalysisResult* analysisResult, const void* analysisConfiguration);

#if defined(__cplusplus)
}
#endif

#endif
