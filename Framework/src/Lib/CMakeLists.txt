target_sources(${LIB_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLib.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncUtil.hpp
)

# Add sub directories.
add_subdirectory(impl)