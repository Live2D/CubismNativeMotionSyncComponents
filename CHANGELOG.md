# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).


## [Unreleased]

### Fixed

* Implement support for `MSVC19.40` in the VS2022 build. by [@tatsuya-shiozawa](https://github.com/Live2D/CubismNativeMotionSyncComponents/pull/3)


## [5-r.1] - 2024-05-30

### Added

* Add right orientation to the fixed orientations in the iOS and Android app.

### Changed

* Disabled `stat` due to effect of Apple Privacy Manifests.
* Cleaned up the include directives.
* Change functions used to ensure line breaks are done correctly.
* Change the usage part of function `fmod()` to function `CubismMath.ModF()`.
* Change the Japanese sound file to one with a sampling frequency of 48 kHz.
* Change so that the sound file does not automatically play when loading the model.
* `MotionSyncAnalysisConfig_CRI::AudioLevelEffectRatio` has been marked as `Unused`.

### Fixed

* Fix `EXC_BAD_ACCESS` when running on iOS.
  * The reason is that recording and playback are separate threads and access the buffer at the same time. 
  * Therefore, mutexes were used to prevent simultaneous access.
* Fix an issue that sample audio output is only for speakers on iOS.
* Fix the method of locking the app to prevent the model from being cut off on the iPad.

### Removed

* Remove `CubismMotionSync::SetAudioLevelEffectRatio` function.


## [5-r.1-beta.2] - 2024-01-25

### Added

* Add a sample application that can be run on Android and iOS.
* Add sample by microphone input.

### Changed

* Remove the `DEFAULT_AUDIO_BIT_DEPTH` macro.
* Change to function name of `LAppMotionSyncModel`.
* Change `LAppPlaySound` to `LAppAudioManager`.
* Optimize type casting.

### Fixed

* Fix build failure in VS2013.
* Fix to not get empty characters when getting audio file names.
* Fix crash when `motionsync3.json` is not found.
* Fix the MotionSync analysis process.


## [5-r.1-beta.1] - 2023-10-17

### Added

* New released!


[5-r.1]: https://github.com/Live2D/CubismNativeMotionSyncComponentsDev/compare/5-r.1-beta.2...5-r.1
[5-r.1-beta.2]: https://github.com/Live2D/CubismNativeMotionSyncComponentsDev/compare/5-r.1-beta.1...5-r.1-beta.2
