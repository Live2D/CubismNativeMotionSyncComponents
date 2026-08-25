[English](README.md) / [日本語](README.ja.md)

# Live2D Cubism MotionSync Core

This folder contains header files and platform-specific library files for developing native applications with using the motion sync.


## Library List

| Platform | Architecture | dll | lib | Path | Note |
| --- | --- | --- | --- | --- | --- |
| Android | ARM64 | ✓ |   | CRI/dll/Android/arm64-v8a |   |
| Android | x86 | ✓ |   | CRI/dll/Android/x86 |   |
| Android | x86_64 | ✓ |   | CRI/dll/Android/x86_64 |   |
| iOS | ARM64 |   | ✓ | CRI/lib/iOS/Release-iphoneos | iOS Devices |
| iOS | ARM64 |   | ✓ | CRI/lib/iOS/Release-iphonesimulator | iOS Simulator |
| iOS | x86_64 |   | ✓ | CRI/lib/iOS/Release-iphonesimulator | iOS Simulator |
| macOS | ARM64 | ✓ |   | CRI/dll/macOS |   |
| macOS | x86_64 | ✓ |   | CRI/dll/macOS |   |
| Windows | x86 | ✓ |   | CRI/dll/Windows/x86 |   |
| Windows | x86_64 | ✓ |   | CRI/dll/Windows/x86_64 |   |


### Calling convention

When using the dynamic library of *Windows/x86*, explicitly use `__stdcall` as the calling convention.

---

[![CRIWARE for Games](CRIWARELOGO_1.png)](https://game.criware.jp/)

Powered by "CRIWARE".CRIWARE is a trademark of CRI Middleware Co., Ltd.
