set(NATIVE_SRC_PATH $CACHE{NATIVE_PATH}/Samples/OpenGL/Demo/proj.android.cmake/Full/app/src/main/cpp)

target_include_directories(${APP_NAME} PUBLIC ${NATIVE_SRC_PATH})

target_sources(${APP_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/JniBridgeC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JniBridgeC.hpp
    ${NATIVE_SRC_PATH}/LAppAllocator.cpp
    ${NATIVE_SRC_PATH}/LAppAllocator.hpp
    ${NATIVE_SRC_PATH}/LAppDefine.cpp
    ${NATIVE_SRC_PATH}/LAppDefine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppDelegate.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncDefine.hpp
    ${NATIVE_SRC_PATH}/LAppPal.cpp
    ${NATIVE_SRC_PATH}/LAppPal.hpp
    ${NATIVE_SRC_PATH}/LAppSprite.cpp
    ${NATIVE_SRC_PATH}/LAppSprite.hpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
    ${NATIVE_SRC_PATH}/LAppWavFileHandler.cpp
    ${NATIVE_SRC_PATH}/LAppWavFileHandler.hpp
)
