if(NOT "$CACHE{CSM_MOTIONSYNC_USE_STATIC_LIB}" STREQUAL "TRUE")
  target_sources(${LIB_NAME}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncConfig.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/ICubismMotionSyncEngineLib.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLib.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLib.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncUtil.hpp
  )
else()
  target_sources(${LIB_NAME}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncConfig.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/ICubismMotionSyncEngineLib.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLib.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLib.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncUtil.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineStaticLib.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineStaticLib.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/Live2DCubismMotionSyncEngine.hpp
  )
  target_compile_definitions(${LIB_NAME} PRIVATE CSM_MOTIONSYNC_USE_STATIC_LIB="")
endif()

# Add sub directories.
add_subdirectory(impl)
