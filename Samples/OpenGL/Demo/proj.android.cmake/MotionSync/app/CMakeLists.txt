cmake_minimum_required(VERSION 3.10)

# Set app name.
set(APP_NAME Demo)
# Set directory paths.
# SDK_ROOT_PATH and NATIVE_DIR are defined in the build.gradle.
set(NATIVE_PATH ${NATIVE_DIR} CACHE PATH [[Native File Path]] FORCE)
set(NATIVE_CORE_PATH $CACHE{NATIVE_PATH}/Core)
set(MOTIONSYNC_CRI_CORE_PATH ${SDK_ROOT_PATH}/Core/CRI/dll/Android)
set(NATIVE_FRAMEWORK_PATH $CACHE{NATIVE_PATH}/Framework)
set(MOTIONSYNC_FRAMEWORK_PATH ${SDK_ROOT_PATH}/Framework)
set(THIRD_PARTY_PATH $CACHE{NATIVE_PATH}/Samples/OpenGL/thirdParty)
set(STB_PATH ${THIRD_PARTY_PATH}/stb)

# Specify version of compiler.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Cubism Core.
# Import as static library.
add_library(Live2DCubismCore STATIC IMPORTED)
# Find library path.
set_target_properties(Live2DCubismCore
  PROPERTIES
    IMPORTED_LOCATION
      ${NATIVE_CORE_PATH}/lib/android/${ANDROID_ABI}/libLive2DCubismCore.a
    INTERFACE_INCLUDE_DIRECTORIES ${NATIVE_CORE_PATH}/include
)


# Specify Cubism Framework rendering.
set(FRAMEWORK_SOURCE OpenGL)
# Add Cubism Native Framework.
add_subdirectory(${NATIVE_FRAMEWORK_PATH} ${CMAKE_CURRENT_BINARY_DIR}/Framework)
# Add rendering definition to framework.
target_compile_definitions(Framework PUBLIC CSM_TARGET_ANDROID_ES2)

# Add Cubism Native Framework.
add_subdirectory(${MOTIONSYNC_FRAMEWORK_PATH} ${CMAKE_CURRENT_BINARY_DIR}/MotionSyncFramework)
# Set macro to MotionSync Framework.
target_compile_definitions(MotionSyncFramework PRIVATE MOTIONSYNC_CORE_CRI_LIB_PATH="")

# Make library for android app.
add_library(${APP_NAME} SHARED)
# Add source files.
add_subdirectory(src/main/cpp)
# Link libraries to app.
target_link_libraries(${APP_NAME}
  Framework
  MotionSyncFramework
  Live2DCubismCore
  GLESv2
  log
)
# Specify include directories.
target_include_directories(${APP_NAME} PRIVATE ${STB_PATH})
