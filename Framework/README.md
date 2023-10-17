[English](README.md) / [日本語](README.ja.md)

---

# Cubism Native MotionSync Framework

This is a framework for using models that utilize the motion-sync functionality produced in Live2D Cubism Editor within your application. 
It offers various features for using the motion sync functionality employed by the model.

The display and manipulation features of the model are provided by the [Cubism Native Framework](https://github.com/Live2D/CubismNativeFramework).


## License

Please read the [license](LICENSE.md) before use.


## Components

### Lib

This is a component for making the Live2D Cubism MotionSync Core library available in the framework, allowing you to use the motion sync feature.


## Cubism SDK for Native

This plugin is a Cubism SDK plugin for the Cubism SDK for Native.

The Cubism SDK for Native package is required for use.

If you are looking for the download page of the SDK package, please go to the [download page](https://www.live2d.com/sdk/download/native/).


## Live2D Cubism Core for Native

A library for loading the model.
Live2D Cubism Core for Native is not included in this repository.

To download, please refer to [this](https://www.live2d.com/download/cubism-sdk/download-native/) page.
Copy the contents of the downloaded ZIP file to the `Core` directory of this repository and link it to the program.


## Live2D Cubism MotionSync Core

This library provides the motion-sync function. Live2D Cubism MotionSync Core is not included in this repository.

To download the plugin package that includes Live2D Cubism MotionSync Core, please refer to [this](https://www.live2d.com/sdk/download/motionsync/) page.


## Sample

For examples of standard application implementations, please refer to the Samples directory.


## SDK manual

[Cubism SDK Manual](https://docs.live2d.com/en/cubism-sdk-manual/top/)


## Changelog

Please refer to [CHANGELOG.md](CHANGELOG.md) for the changelog of this repository.


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
