target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../main/cpp)

target_sources(${APP_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppMotionSyncModel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppAudioManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LAppView.hpp
)
