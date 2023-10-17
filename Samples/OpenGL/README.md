[English](README.md) / [日本語](README.ja.md)

---

# Cubism Native MotionSync Samples for OpenGL

This is a sample implementation of an application that handles motion-sync functionality implemented in OpenGL.


## Development environment

| Third Party | Version |
| --- | --- |
| [GLEW] | 2.2.0 |
| [GLFW] | 3.3.8 |
| [stb_image.h] | 2.28 |

For other development environments and operation environments, see [README.md](/README.md) in the top directory.


## Directory structure

```
.
└─ Demo
   ├─ proj.mac.cmake        # CMake project for macOS
   └─ proj.win.cmake        # CMake project for Windows
```


## Demo

This is a sample that supports motion-sync.

The items contained in this directory are as follows.

### proj.mac.cmake

CMake project for macOS.

Running the script in the `script` directory will generate a CMake deliverable in the `build` directory.

| Script name | Product |
| --- | --- |
| `make_xcode` | Executable application |
| `proj_xcode` | Xcode project |

### proj.win.cmake

CMake project for Windows.

Running the script in the `script` directory will generate a CMake deliverable in the `build` directory.

| Script name | Product |
| --- | --- |
| `nmake_msvcXXXX.bat` | Executable application |
| `proj_msvcXXXX.bat` | Visual Studio project |
