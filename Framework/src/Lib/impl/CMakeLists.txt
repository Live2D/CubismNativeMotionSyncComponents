if(WIN32)
    set(LIBRARY_LOADER "Win")
else()
    set(LIBRARY_LOADER "Other")
endif()

target_sources(${LIB_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CubismMotionSyncEngineLoader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LibraryLoader_${LIBRARY_LOADER}.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LibraryLoader.h
)
