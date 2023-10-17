set(SDK_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../..)
set(NATIVE_SRC_PATH $CACHE{NATIVE_PATH}/Samples/OpenGL/Demo/proj.win.cmake/src)

target_include_directories(${APP_NAME} PUBLIC ${NATIVE_SRC_PATH})

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
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppPlaySound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppPlaySound.hpp
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
