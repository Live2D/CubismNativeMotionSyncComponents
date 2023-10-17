# Live2D Cubism MotionSync Engine

This folder contains header files and platform-specific library files for using the motion sync plug-in.


## Library List

| Platform | Architecture | dll | lib | Path | Note |
| --- | --- | --- | --- | --- | --- |
| macOS | ARM64 | ✓ |   | macos/arm64 |   |
| macOS | x86_64 | ✓ |   | macos/x86_64 |   |
| Windows | x86 | ✓ |   | windows/x86 |   |
| Windows | x86_64 | ✓ |   | windows/x86_64 |   |


### Calling convention

When using the dynamic library of *Windows/x86*, explicitly use `__stdcall` as the calling convention.

---

[![CRIWARE for Games](CRIWARELOGO_1.png)](https://game.criware.jp/)

Powered by "CRIWARE".CRIWARE is a trademark of CRI Middleware Co., Ltd.
