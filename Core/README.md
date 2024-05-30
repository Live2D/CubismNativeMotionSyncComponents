[English](README.md) / [日本語](README.ja.md)

# Live2D Cubism MotionSync Core

This folder contains header files and platform-specific library files for developing native applications with using the motion sync.


## Library List

| Platform | Architecture | dll | lib | Path | Note |
| --- | --- | --- | --- | --- | --- |
| Android | ARM64 | ✓ |   | Android/arm64-v8a |   |
| Android | ARMv7 | ✓ |   | Android/armeabi-v7a | This library is currently deprecated and will be removed in the near future. |
| Android | x86 | ✓ |   | Android/x86 |   |
| Android | x86_64 | ✓ |   | Android/x86_64 |   |
| iOS | ARM64 |   | ✓ | iOS/xxx-iphoneos | iOS Devices |
| iOS | x86_64 |   | ✓ | iOS/xxx-iphonesimulator | iOS Simulator |
| macOS | ARM64 | ✓ |   | macOS |   |
| macOS | x86_64 | ✓ |   | macOS |   |
| Windows | x86 | ✓ |   | Windows/x86 |   |
| Windows | x86_64 | ✓ |   | Windows/x86_64 |   |


### Calling convention

When using the dynamic library of *Windows/x86*, explicitly use `__stdcall` as the calling convention.

---

[![CRIWARE for Games](CRIWARELOGO_1.png)](https://game.criware.jp/)

Powered by "CRIWARE".CRIWARE is a trademark of CRI Middleware Co., Ltd.
