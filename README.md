[English](README.md) / [日本語](README.ja.md)


# Cubism MotionSync Plugin for Native

This is a Cubism SDK plugin for using the motion-sync function on the Cubism SDK for Native.


## License

Please read the [license](LICENSE.md) before use.


## Notice

Please read the [notices](NOTICE.md) before use.


## Structure

### Dependencies

#### Cubism SDK for Native

This plugin is a Cubism SDK plugin for the latest Cubism SDK for Native.

The Cubism SDK for Native package is required for use.

If you are looking for the download page of the SDK package, please go to the [download page](https://www.live2d.com/en/sdk/download/native/).

Please place the Cubism SDK for Native package at the same level as this repository.

#### Live2D Cubism MotionSync Core

This library provides the motion-sync function. Live2D Cubism MotionSync Core is not included in this repository.

To download the plugin package that includes Live2D Cubism MotionSync Core, please refer to [this](https://www.live2d.com/en/sdk/download/motionsync/) page.


## Directory structure

```
.
├─ Core                # Directory containing Live2D Cubism Motion Sync Core
├─ Framework           # Directory containing source code for rendering and animation functions for motion-sync
└─ Samples
   └─ OpenGL           # Directory containing the OpenGL sample project
```


## How to build

The build method depends on the sample project.
Please refer to the `README.md` included with each sample project.

### Where to generate the sample project deliverables

In this sample, the deliverables of CMake projects except Android are generated in the `bin` directory.

E.g. When building a macOS project of the OpenGL sample using the `make_gcc` script
```
Demo
└─ proj.mac.cmake
   └─ build
      └─ make_gcc
         └─ bin
            └─ Demo
               ├─ Resources    # Same as Samples/Resources
               └─ Demo         # Executable applications
```


## SDK manual

[Cubism SDK Manual](https://docs.live2d.com/en/cubism-sdk-manual/top/)


## Changelog

Please refer to [CHANGELOG.md](CHANGELOG.md) for the changelog of this repository.


## Development environment

| Development Tool | Version |
| --- | --- |
| Android Studio | Hedgehog 2023.1.1 |
| CMake | 3.28.1 |
| Visual Studio 2013 | Update 5 |
| Visual Studio 2015 | Update 3 |
| Visual Studio 2017 | 15.9.59 |
| Visual Studio 2019 | 16.11.33 |
| Visual Studio 2022 | 17.8.4 |
| Xcode | 15.1 |

### Android

| Android SDK tools | Version |
| --- | --- |
| Android NDK | 25.2.9519653 |
| Android SDK | 34.0.0 |
| CMake | 3.22.1 |


## Operation environment

| Platform | Version |
| --- | --- |
| iOS / iPadOS | 16.6.1 |
| macOS | 14.1.1 |
| Windows 10 | 22H2 |
| Windows 11 | 22H2 |

### Android

| Version | Device | Tegra |
| --- | --- | --- |
| 13 | Pixel 7a | |
| 7.1.1 | Nexus 9 | ✔︎ |


## Sound device

The sound device for input/output is a specification whereby the default device is used.

In this sample, the input sound is played directly through the device's sound playback system.
 Depending on your environment, feedback may occur. To prevent this, please mute the sample application and the device's sound playback, or move the microphone and speaker away from each other.


## Contribution to the project

There are many ways to contribute to the project: logging bugs, submitting pull requests on this GitHub, and reporting issues and making suggestions in Live2D Community.

### Forking and Pull Requests

We very much appreciate your pull requests, whether they bring fixes, improvements, or even new features. Note, however, that the wrapper is designed to be as lightweight and shallow as possible and should therefore only be subject to bug fixes and memory/performance improvements. To keep the main repository as clean as possible, please create a personal fork and feature branch as needed.

### Bugs

We are regularly checking bug reports and feature requests at Live2D Community. Before submitting a bug report, please search the Live2D Community to see if the bug report or feature request has already been submitted. Add your comment to the relevant issue if it already exists.

### Suggestions

We're also interested in your feedback for the future of the SDK. You can submit a suggestion or feature request at Live2D Community. To make this process more effective, we're asking that you include more information to help define them more clearly.

## Coding Guidelines

### Naming

Try to stick to the [Microsoft guidelines](https://msdn.microsoft.com/en-us/library/ms229002(v=vs.110).aspx) whenever possible. Private fields are named in lowercase, starting with an underscore.

### Style

- In the Unity Editor extension, try to write expressive code with LINQ and all the other fancy stuff.
- Otherwise, do not use LINQ and prefer `for` to `foreach`.
- Try to make the access modifier explicit. Use `private void Update()` instead of `void Update()`.

## Forum

If you have any questions, please join the official Live2D forum and discuss with other users.

- [Live2D Creator's Forum](https://community.live2d.com/)
- [Live2D 公式クリエイターズフォーラム (Japanese)](https://creatorsforum.live2d.com/)
