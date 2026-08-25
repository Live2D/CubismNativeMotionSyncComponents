target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../main/cpp)

target_sources(${APP_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/JniBridgeC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JniBridgeC.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkModel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkView.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionEventDefine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncLinkDelegate.hpp
)
