﻿[English](README.md) / [日本語](README.ja.md)

---


# Cubism MotionSync Plugin for Native

Cubism SDK for Native上で、モーションシンク機能を利用するためのCubism SDKプラグインです。


## ライセンス

本 SDK を使用する前に[ライセンス](LICENSE.md)をご確認ください。


## お知らせ

本 SDK を使用する前に [お知らせ](NOTICE.ja.md)をご確認ください。


## 構造

### 依存関係

#### Cubism SDK for Native

Live2D Cubism Editor で出力したモーションシンク対応モデルを表示するアプリケーションのサンプル実装です。

利用するには最新のCubism SDK for Nativeのパッケージが必要となります。
SDKパッケージのダウンロードページをお探しの場合は、[ダウンロードページ](https://www.live2d.com/sdk/download/native/)にアクセスしてください。
このリポジトリと同じ階層にCubism SDK for Nativeのパッケージを置いてください。

#### Live2D Cubism MotionSync Core

モーションシンク機能を提供するライブラリです。当リポジトリにはLive2D Cubism MotionSync Coreは同梱されていません。

Live2D Cubism MotionSync Coreを同梱したプラグインパッケージをダウンロードするには[こちら](https://www.live2d.com/sdk/download/motionsync/)のページを参照ください。


## Cubism 5新機能や過去バージョンとの互換性について

本 SDK はCubism 5に対応した製品です。  
Cubism 5 Editorに搭載された新機能のSDK対応については [こちら](https://docs.live2d.com/cubism-sdk-manual/cubism-5-new-functions/)をご確認ください。  
過去バージョンのCubism SDKとの互換性については [こちら](https://docs.live2d.com/cubism-sdk-manual/compatibility-with-cubism-5/)をご確認ください。


## ディレクトリ構成

```
.
├─ Core                # Live2D Cubism Motion Sync Core が含まれるディレクトリ
├─ Framework           # Motion Sync用のレンダリングやアニメーション機能などのソースコードが含まれるディレクトリ
└─ Samples
   └─ OpenGL           # OpenGL のサンプルプロジェクトが含まれるディレクトリ
```


## ビルド方法

ビルド方法についてはサンプルプロジェクトによって異なります。
各サンプルプロジェクトに同梱された `README.md` を参照ください。


### サンプルプロジェクトの成果物の生成場所

本サンプルでは、CMake プロジェクトの成果物は `bin` ディレクトリに生成されます。

例）OpenGL サンプルの macOS プロジェクトのビルドを `make_gcc` スクリプトを使用して行なった場合
```
Demo
└─ proj.mac.cmake
   └─ build
      └─ make_gcc
         └─ bin
            └─ Demo
               ├─ Resources    # Samples/Resources と同じ
               └─ Demo         # 実行可能なアプリケーション
```


## SDKマニュアル

[Cubism SDK Manual](https://docs.live2d.com/cubism-sdk-manual/top/)


## 変更履歴

当リポジトリの変更履歴については [CHANGELOG.md](CHANGELOG.md) を参照ください。


## 開発環境

| 開発ツール | バージョン |
| --- | --- |
| Android Studio | Meerkat 2024.3.1 |
| CMake | 3.31.6 |
| Visual Studio 2015 | Update 3 |
| Visual Studio 2017 | 15.9.62 |
| Visual Studio 2019 | 16.11.45 |
| Visual Studio 2022 | 17.13.3 |
| Xcode | 16.2 |

### Android

| Android SDK tools | バージョン |
| --- | --- |
| Android NDK | 26.3.11579264 |
| Android SDK | 35.0.2 |
| CMake | 3.22.1 |


## 動作確認環境

| プラットフォーム | バージョン |
| --- | --- |
| iOS / iPadOS | 18.3.2 |
| macOS | 18.3.1 |
| Windows 10 | 22H2 |
| Windows 11 | 24H2 |

### Android

| バージョン | デバイス | Tegra |
| --- | --- | --- |
| 15 | Pixel 7a, Xperia 10 V | |
| 7.1.1 | Nexus 9 | ✔︎ |

### Cubism SDK for Native

[Cubism 5 SDK for Native R3](https://github.com/Live2D/CubismNativeSamples/releases/tag/5-r.3)


## サウンドデバイス

入出力用のサウンドデバイスは、既定のデバイスが使用される仕様です。

本サンプルでは入力した音声がデバイスの音声再生機器へそのまま再生されます。
ご利用の環境によってはハウリングする場合がございますので、サンプルアプリケーションやデバイスの音声再生機器を一度ミュートにするか、マイクとスピーカーの位置を離してご利用ください。


## プロジェクトへの貢献

プロジェクトに貢献する方法はたくさんあります。バグのログの記録、このGitHubでのプルリクエストの送信、Live2Dコミュニティでの問題の報告と提案の作成です。


### フォークとプルリクエスト

修正、改善、さらには新機能をもたらすかどうかにかかわらず、プルリクエストに感謝します。メインリポジトリを可能な限りクリーンに保つために、必要に応じて個人用フォークと機能ブランチを作成してください。


### バグ

Live2Dフォーラムでは、問題のレポートと機能リクエストを定期的にチェックしています。バグレポートを提出する前に、Live2Dコミュニティで検索して、問題のレポートまたは機能リクエストがすでに投稿されているかどうかを確認してください。問題がすでに存在する場合は、関連するコメントを追記してください。


### 提案

SDKの将来についてのフィードバックにも関心があります。Live2Dフォーラムで提案や機能のリクエストを送信できます。このプロセスをより効果的にするために、それらをより明確に定義するのに役立つより多くの情報を含めるようお願いしています。


## フォーラム

ユーザー同士でCubism SDKの活用方法の提案や質問をしたい場合は、是非公式クリエイターズフォーラムをご活用ください。

- [Live2D 公式クリエイターズフォーラム](https://creatorsforum.live2d.com/)
- [Live2D Creators Forum(English)](https://community.live2d.com/)
