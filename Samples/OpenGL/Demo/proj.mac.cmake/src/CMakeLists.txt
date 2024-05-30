set(NATIVE_SRC_PATH $CACHE{NATIVE_PATH}/Samples/OpenGL/Demo/proj.mac.cmake/src)

target_include_directories(${APP_NAME} PUBLIC ${NATIVE_SRC_PATH})

if (CSM_MOTIONSYNC_MICROPHONE_DEMO)
  target_sources(${APP_NAME}
    PRIVATE
      ${NATIVE_SRC_PATH}/LAppAllocator.cpp
      ${NATIVE_SRC_PATH}/LAppAllocator.hpp
      ${NATIVE_SRC_PATH}/LAppDefine.cpp
      ${NATIVE_SRC_PATH}/LAppDefine.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMicrophoneDelegate.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMicrophoneDelegate.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncMicrophoneModel.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncMicrophoneModel.hpp
      ${NATIVE_SRC_PATH}/LAppPal.cpp
      ${NATIVE_SRC_PATH}/LAppPal.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMutex.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMutex.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMicrophoneAudioManager.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMicrophoneAudioManager.hpp
      ${NATIVE_SRC_PATH}/LAppSprite.cpp
      ${NATIVE_SRC_PATH}/LAppSprite.hpp
      ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
      ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
      ${NATIVE_SRC_PATH}/LAppWavFileHandler.cpp
      ${NATIVE_SRC_PATH}/LAppWavFileHandler.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/mainMotionSyncMicrophone.cpp
      ${NATIVE_SRC_PATH}/CubismSampleViewMatrix.cpp
      ${NATIVE_SRC_PATH}/CubismSampleViewMatrix.hpp
  )
else ()
  target_sources(${APP_NAME}
    PRIVATE
      ${NATIVE_SRC_PATH}/LAppAllocator.cpp
      ${NATIVE_SRC_PATH}/LAppAllocator.hpp
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
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.hpp
      ${NATIVE_SRC_PATH}/LAppSprite.cpp
      ${NATIVE_SRC_PATH}/LAppSprite.hpp
      ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
      ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
      ${NATIVE_SRC_PATH}/LAppWavFileHandler.cpp
      ${NATIVE_SRC_PATH}/LAppWavFileHandler.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/mainMotionSync.cpp
      ${NATIVE_SRC_PATH}/CubismSampleViewMatrix.cpp
      ${NATIVE_SRC_PATH}/CubismSampleViewMatrix.hpp
  )
endif ()
