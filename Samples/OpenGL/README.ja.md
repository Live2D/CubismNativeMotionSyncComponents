[English](README.md) / [日本語](README.ja.md)

---

# Cubism Native MotionSync Samples for OpenGL

OpenGL で実装したモーションシンク機能を扱うアプリケーションのサンプル実装です。


## 開発環境

| サードパーティ | バージョン |
| --- | --- |
| [GLEW] | 2.2.0 |
| [GLFW] | 3.3.9 |
| [ios-cmake] | 4.4.1 |
| [stb_image.h] | 2.29 |

その他の開発環境・動作確認環境はトップディレクトリにある [README.md](/README.ja.md) を参照してください。


## ディレクトリ構造

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

モーションシンクに対応したサンプルです。
音声ファイルを利用する音声ファイルバージョンのサンプルと、マイク入力に対応したマイク入力バージョンのサンプルの二通りがあります。
選択方法については各プラットフォームの項目を参照してください。

音声ファイルバージョンでは音声ファイルによるモーションシンクを行います。
また、メニューボタンから音声ファイルを切り替えることができます。

マイク入力バージョンではマイク入力によるモーションシンクを行います。

このディレクトリ内に含まれるものは以下の通りです。

### proj.android.cmake

このディレクトリにはモーションシンクの Android Studio プロジェクトが含まれます。
本体入出力限定のサンプルになっています。
Build Variantsを変更すると、音声ファイルのサンプルかマイク入力のサンプルに変更できます。

NOTE: 事前に下記の SDK のダウンロードが必要です

* Android SDK Build-Tools
* NDK
* CMake

### proj.ios.cmake

iOS 用の CMake プロジェクトです。
本体入出力限定のサンプルになっています。

`script` ディレクトリのスクリプトを実行すると `build` ディレクトリに CMake 成果物が生成されます

| スクリプト名 | 生成物 |
| --- | --- |
| `proj_xcode` | Xcode プロジェクト |

これらのスクリプトを実行した際に音声ファイルのサンプルを生成するか、マイク入力のサンプルを生成するかを選択することができます。

Appleシリコン搭載のMacでのiOS Simulator用ビルド時は下記の手順を行ってください。

1. Xcode の `Project設定 - TARGETS - Demo` および `Framework` の `Build Settings - Architectures - Architectures` の記載を実行環境に合わせて変更してください。


CMake のツールチェーンとして [ios-cmake] を使用しています。
[thirdParty](#thirdParty) の項目を参照して事前にダウンロードを行なってください。

[ios-cmake]: https://github.com/leetal/ios-cmake

### proj.mac.cmake

macOS 用の CMake プロジェクトです。

`script` ディレクトリのスクリプトを実行すると `build` ディレクトリに CMake 成果物が生成されます

| スクリプト名 | 生成物 |
| --- | --- |
| `make_xcode` | 実行可能なアプリケーション |
| `proj_xcode` | Xcode プロジェクト |

これらのスクリプトを実行した際に音声ファイルのサンプルを生成するか、マイク入力のサンプルを生成するかを選択することができます。

追加ライブラリとして [GLEW] と [GLFW] を使用しています。
[thirdParty](#thirdParty) の項目を参照して事前にダウンロードを行なってください。

### proj.win.cmake

Windows 用の CMake プロジェクトです。

`script` ディレクトリのスクリプトを実行すると `build` ディレクトリに CMake 成果物が生成されます

| スクリプト名 | 生成物 |
| --- | --- |
| `nmake_msvcXXXX.bat` | 実行可能なアプリケーション |
| `proj_msvcXXXX.bat` | Visual Studio プロジェクト |

これらのスクリプトを実行した際に音声ファイルのサンプルを生成するか、マイク入力のサンプルを生成するかを選択することができます。

追加ライブラリとして [GLEW] と [GLFW] を使用しています。
[thirdParty](#thirdParty) の項目を参照して事前にダウンロードを行なってください。

## thirdParty

Cubism Native Samples for OpenGLと同様にサードパーティライブラリを展開するスクリプトを事前に実行する必要があります。
詳しいセットアップ方法はCubism Native Samples for OpenGLの `README.ja.md` をご確認ください。

[GLEW]: https://github.com/nigels-com/glew
[GLFW]: https://github.com/glfw/glfw
[ios-cmake]: https://github.com/leetal/ios-cmake
[stb_image.h]: https://github.com/nothings/stb/blob/master/stb_image.h
