if(WIN32)
    set(LIBRARY_LOADER "Win")
elseif(CMAKE_HOST_APPLE)
    set(LIBRARY_LOADER "Mac")
else()

endif()

target_sources(${LIB_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLoader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LibraryLoader_${LIBRARY_LOADER}.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LibraryLoader.h
)
