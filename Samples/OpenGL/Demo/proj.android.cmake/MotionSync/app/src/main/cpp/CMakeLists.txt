set(NATIVE_SRC_PATH $CACHE{NATIVE_PATH}/Samples/OpenGL/Demo/proj.android.cmake/Full/app/src/main/cpp)

# HACK: If the order of target_include_directories is changed, the build will not pass due to the search order of the header files.
# Due to the naming conflict in LAppView.hpp and the presence or absence of InitializeShader declarations.
target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../${CSM_MOTIONSYNC_FLAVOR_DEMO}/cpp)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncModel.hpp
    ${NATIVE_SRC_PATH}/LAppPal.cpp
    ${NATIVE_SRC_PATH}/LAppPal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.hpp
    ${NATIVE_SRC_PATH}/LAppSprite.cpp
    ${NATIVE_SRC_PATH}/LAppSprite.hpp
    ${NATIVE_SRC_PATH}/LAppSpriteShader.cpp
    ${NATIVE_SRC_PATH}/LAppSpriteShader.hpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.cpp
    ${NATIVE_SRC_PATH}/LAppTextureManager.hpp
    ${NATIVE_SRC_PATH}/LAppWavFileHandler.cpp
    ${NATIVE_SRC_PATH}/LAppWavFileHandler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppView.hpp
)
