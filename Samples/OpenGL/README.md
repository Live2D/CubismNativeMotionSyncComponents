[English](README.md) / [日本語](README.ja.md)

---

# Cubism Native MotionSync Samples for OpenGL

This is a sample implementation of an application that handles motion-sync functionality implemented in OpenGL.


## Development environment

| Third Party | Version |
| --- | --- |
| [GLEW] | 2.2.0 |
| [GLFW] | 3.3.9 |
| [ios-cmake] | 4.4.1 |
| [stb_image.h] | 2.29 |

For other development environments and operation environments, see [README.md](/README.md) in the top directory.


## Directory structure

```
.
└─ Demo
   ├─ proj.android.cmake
   │  └─ MotionSync         # Android Studio project
   ├─ proj.ios.cmake        # CMake project for iOS
   ├─ proj.mac.cmake        # CMake project for macOS
   └─ proj.win.cmake        # CMake project for Windows
```


## Demo

This is a sample that supports motionSync.

The items contained in this directory are as follows.

### proj.android.cmake

This directory contains the Android Studio project for MotionSync.
The sample is limited to body input/output.
Change Build Variants to the sound file version or the microphone version of the sample.

NOTE: You need to download the following SDKs in advance.

* Android SDK Build-Tools
* NDK
* CMake

### proj.ios.cmake

CMake project for iOS.
The sample is limited to body input/output.

Running the script in the `script` directory will generate a CMake deliverable in the `build` directory.

| Script name | Product |
| --- | --- |
| `proj_xcode` | Xcode project |

When you run these scripts, you can choose to generate the sound file version or the microphone version of the sample.

Follow the steps below when building for iOS Simulator on a Mac with Apple silicon.

1. Change the description of Xcode's `Project Settings - TARGETS - Demo` and `Framework`'s `Build Settings - Architectures - Architectures` according to the execution environment.


[ios-cmake] is used as a toolchain for CMake.
Please refer to the [thirdParty](#thirdParty) section and download in advance.

[ios-cmake]: https://github.com/leetal/ios-cmake

### proj.mac.cmake

CMake project for macOS.

Running the script in the `script` directory will generate a CMake deliverable in the `build` directory.

| Script name | Product |
| --- | --- |
| `make_xcode` | Executable application |
| `proj_xcode` | Xcode project |

When you run these scripts, you can choose to generate the sound file version or the microphone version of the sample.

[GLEW] and [GLFW] are used as additional libraries.
Please refer to the [thirdParty](#thirdParty) section and download in advance.

### proj.win.cmake

CMake project for Windows.

Running the script in the `script` directory will generate a CMake deliverable in the `build` directory.

| Script name | Product |
| --- | --- |
| `nmake_msvcXXXX.bat` | Executable application |
| `proj_msvcXXXX.bat` | Visual Studio project |

When you run these scripts, you can choose to generate the sound file version or the microphone version of the sample.

[GLEW] and [GLFW] are used as additional libraries.
Please refer to the [thirdParty](#thirdParty) section and download in advance.

## thirdParty

As with Cubism Native Samples for OpenGL, a script to deploy third-party libraries must be run in advance.
Please refer to `README.en.md` of Cubism Native Samples for OpenGL for detailed setup instructions.

[GLEW]: https://github.com/nigels-com/glew
[GLFW]: https://github.com/glfw/glfw
[ios-cmake]: https://github.com/leetal/ios-cmake
[stb_image.h]: https://github.com/nothings/stb/blob/master/stb_image.h
