set(NATIVE_SRC_PATH ${NATIVE_PATH}/Samples/OpenGL/Demo/proj.ios.cmake/src)

target_include_directories(${APP_NAME} PUBLIC ${NATIVE_SRC_PATH})

if (CSM_MOTIONSYNC_MICROPHONE_DEMO)
  target_sources(${APP_NAME}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/AppMicrophoneDelegate.h
      ${CMAKE_CURRENT_SOURCE_DIR}/AppMicrophoneDelegate.mm
      ${NATIVE_SRC_PATH}/LAppAllocator.h
      ${NATIVE_SRC_PATH}/LAppAllocator.mm
      ${NATIVE_SRC_PATH}/LAppDefine.h
      ${NATIVE_SRC_PATH}/LAppDefine.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.h
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncMicrophoneModel.h
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncMicrophoneModel.mm
      ${NATIVE_SRC_PATH}/LAppPal.h
      ${NATIVE_SRC_PATH}/LAppPal.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMutex.h
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMutex.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMicrophoneAudioManager.h
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMicrophoneAudioManager.mm
      ${NATIVE_SRC_PATH}/LAppSprite.h
      ${NATIVE_SRC_PATH}/LAppSprite.mm
      ${NATIVE_SRC_PATH}/LAppTextureManager.h
      ${NATIVE_SRC_PATH}/LAppTextureManager.mm
      ${NATIVE_SRC_PATH}/LAppWavFileHandler.h
      ${NATIVE_SRC_PATH}/LAppWavFileHandler.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/mainMicrophone.m
      ${CMAKE_CURRENT_SOURCE_DIR}/MicrophoneViewController.h
      ${CMAKE_CURRENT_SOURCE_DIR}/MicrophoneViewController.mm
  )
else ()
  target_sources(${APP_NAME}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/AppDelegate.h
      ${CMAKE_CURRENT_SOURCE_DIR}/AppDelegate.mm
      ${NATIVE_SRC_PATH}/LAppAllocator.h
      ${NATIVE_SRC_PATH}/LAppAllocator.mm
      ${NATIVE_SRC_PATH}/LAppDefine.h
      ${NATIVE_SRC_PATH}/LAppDefine.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.h
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncModel.h
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncModel.mm
      ${NATIVE_SRC_PATH}/LAppPal.h
      ${NATIVE_SRC_PATH}/LAppPal.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.h
      ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.mm
      ${NATIVE_SRC_PATH}/LAppSprite.h
      ${NATIVE_SRC_PATH}/LAppSprite.mm
      ${NATIVE_SRC_PATH}/LAppTextureManager.h
      ${NATIVE_SRC_PATH}/LAppTextureManager.mm
      ${NATIVE_SRC_PATH}/LAppWavFileHandler.h
      ${NATIVE_SRC_PATH}/LAppWavFileHandler.mm
      ${CMAKE_CURRENT_SOURCE_DIR}/main.m
      ${CMAKE_CURRENT_SOURCE_DIR}/ViewController.h
      ${CMAKE_CURRENT_SOURCE_DIR}/ViewController.mm
  )
endif ()
