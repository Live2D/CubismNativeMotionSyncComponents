[English](README.md) / [日本語](README.ja.md)

---

# Cubism Native MotionSync Samples for OpenGL

OpenGL で実装したモーションシンク機能を扱うアプリケーションのサンプル実装です。


## 開発環境

| サードパーティ | バージョン |
| --- | --- |
| [GLEW] | 2.2.0 |
| [GLFW] | 3.3.8 |
| [stb_image.h] | 2.28 |

その他の開発環境・動作確認環境はトップディレクトリにある [README.md](/README.ja.md) を参照してください。


## ディレクトリ構造

```
.
└─ Demo
   ├─ proj.mac.cmake        # CMake project for macOS
   └─ proj.win.cmake        # CMake project for Windows
```


## Demo

モーションシンクに対応したサンプルです。

このディレクトリ内に含まれるものは以下の通りです。

### proj.mac.cmake

macOS 用の CMake プロジェクトです。

`script` ディレクトリのスクリプトを実行すると `build` ディレクトリに CMake 成果物が生成されます

| スクリプト名 | 生成物 |
| --- | --- |
| `make_xcode` | 実行可能なアプリケーション |
| `proj_xcode` | Xcode プロジェクト |


### proj.win.cmake

Windows 用の CMake プロジェクトです。

`script` ディレクトリのスクリプトを実行すると `build` ディレクトリに CMake 成果物が生成されます

| スクリプト名 | 生成物 |
| --- | --- |
| `nmake_msvcXXXX.bat` | 実行可能なアプリケーション |
| `proj_msvcXXXX.bat` | Visual Studio プロジェクト |
