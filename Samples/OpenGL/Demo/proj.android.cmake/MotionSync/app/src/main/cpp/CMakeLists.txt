set(NATIVE_SRC_PATH $CACHE{NATIVE_PATH}/Samples/OpenGL/Demo/proj.android.cmake/Full/app/src/main/cpp)
set(NATIVE_COMMON_SRC_PATH $CACHE{NATIVE_PATH}/Samples/Common)

if (CSM_MOTION_DEMO)
  target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Motion/cpp)
endif()

target_include_directories(${APP_NAME} PUBLIC ${NATIVE_SRC_PATH})
target_include_directories(${APP_NAME} PUBLIC ${NATIVE_COMMON_SRC_PATH})

if (CSM_MOTION_DEMO)
  target_sources(${APP_NAME}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.hpp
      ${NATIVE_COMMON_SRC_PATH}/LAppAllocator_Common.cpp
      ${NATIVE_COMMON_SRC_PATH}/LAppAllocator_Common.hpp
      ${NATIVE_COMMON_SRC_PATH}/LAppWavFileHandler_Common.cpp
      ${NATIVE_COMMON_SRC_PATH}/LAppWavFileHandler_Common.hpp
      ${NATIVE_COMMON_SRC_PATH}/LAppSprite_Common.cpp
      ${NATIVE_COMMON_SRC_PATH}/LAppSprite_Common.hpp
      ${NATIVE_COMMON_SRC_PATH}/LAppTextureManager_Common.cpp
      ${NATIVE_COMMON_SRC_PATH}/LAppTextureManager_Common.hpp
      ${NATIVE_SRC_PATH}/LAppDefine.cpp
      ${NATIVE_SRC_PATH}/LAppDefine.hpp
      ${NATIVE_SRC_PATH}/LAppPal.cpp
      ${NATIVE_SRC_PATH}/LAppPal.hpp
      ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
      ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
      ${NATIVE_SRC_PATH}/LAppSprite.cpp
      ${NATIVE_SRC_PATH}/LAppSprite.hpp
      ${NATIVE_SRC_PATH}/LAppSpriteShader.cpp
      ${NATIVE_SRC_PATH}/LAppSpriteShader.hpp
  )
else()
target_sources(${APP_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/JniBridgeC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JniBridgeC.hpp
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
    ${NATIVE_COMMON_SRC_PATH}/LAppModel_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppModel_Common.hpp
    ${NATIVE_SRC_PATH}/LAppPal.cpp
    ${NATIVE_SRC_PATH}/LAppPal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.hpp
    ${NATIVE_SRC_PATH}/LAppSprite.cpp
    ${NATIVE_SRC_PATH}/LAppSprite.hpp
    ${NATIVE_SRC_PATH}/LAppSpriteShader.cpp
    ${NATIVE_SRC_PATH}/LAppSpriteShader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppView.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppSprite_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppSprite_Common.hpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppTextureManager_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppTextureManager_Common.hpp
    ${NATIVE_COMMON_SRC_PATH}/LAppWavFileHandler_Common.cpp
    ${NATIVE_COMMON_SRC_PATH}/LAppWavFileHandler_Common.hpp
)
endif()
