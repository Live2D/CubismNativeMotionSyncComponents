set(NATIVE_SRC_PATH $CACHE{NATIVE_PATH}/Samples/OpenGL/Demo/proj.mac.cmake/src)
set(NATIVE_COMMON_SRC_PATH $CACHE{NATIVE_PATH}/Samples/Common)

target_include_directories(${APP_NAME} PUBLIC ${NATIVE_COMMON_SRC_PATH})
target_include_directories(${APP_NAME} PUBLIC ${NATIVE_SRC_PATH})

if (CSM_MOTIONSYNC_MOTION_DEMO)
  target_sources(${APP_NAME}
    PRIVATE
    ${NATIVE_COMMON_SRC_PATH}/LAppAllocator_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppAllocator_Common.hpp
    ${NATIVE_SRC_PATH}/LAppDefine.cpp
    ${NATIVE_SRC_PATH}/LAppDefine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionEventDefine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkDelegate.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkModel.hpp
    ${NATIVE_SRC_PATH}/LAppPal.cpp
    ${NATIVE_SRC_PATH}/LAppPal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMutex.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.hpp
    ${NATIVE_SRC_PATH}/LAppSprite.cpp
    ${NATIVE_SRC_PATH}/LAppSprite.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppSprite_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppSprite_Common.hpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppTextureManager_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppTextureManager_Common.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppWavFileHandler_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppWavFileHandler_Common.hpp
    ${NATIVE_SRC_PATH}/LAppSpriteShader.cpp
    ${NATIVE_SRC_PATH}/LAppSpriteShader.hpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainMotionSyncLink.cpp
    ${NATIVE_COMMON_SRC_PATH}/CubismSampleViewMatrix_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/CubismSampleViewMatrix_Common.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppModel_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppModel_Common.hpp
  )
else ()
  target_sources(${APP_NAME}
    PRIVATE
    ${NATIVE_COMMON_SRC_PATH}/LAppAllocator_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppAllocator_Common.hpp
    ${NATIVE_SRC_PATH}/LAppDefine.cpp
    ${NATIVE_SRC_PATH}/LAppDefine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppDelegate.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncModel.hpp
    ${NATIVE_SRC_PATH}/LAppPal.cpp
    ${NATIVE_SRC_PATH}/LAppPal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMutex.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.hpp
    ${NATIVE_SRC_PATH}/LAppSprite.cpp
    ${NATIVE_SRC_PATH}/LAppSprite.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppSprite_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppSprite_Common.hpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppTextureManager_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppTextureManager_Common.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppWavFileHandler_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppWavFileHandler_Common.hpp
    ${NATIVE_SRC_PATH}/LAppSpriteShader.cpp
    ${NATIVE_SRC_PATH}/LAppSpriteShader.hpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainMotionSync.cpp
    ${NATIVE_COMMON_SRC_PATH}/CubismSampleViewMatrix_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/CubismSampleViewMatrix_Common.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppModel_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppModel_Common.hpp
  )
endif ()
