# GDPS Editor 2.2
old gdps editor complete source code 2.2.0.6 - 2.2.12 (archive)
# DISCLAIMER
the developers of this project were beginners at that moment, so this code base contains some bad code and bad practices that should not be replicated

## Requirement
- [Android NDK r16b](https://github.com/android/ndk/wiki/Unsupported-Downloads)

## Setup
- Add Android NDK to your path

## Build Instruction
- Run `build.cmd` or `build.sh`

## Automated builds
- Every push and pull request uploads all native libraries produced in `libs/armeabi-v7a` as a workflow artifact.
- Pushing a tag also creates a draft GitHub release containing all of those native libraries.

## External backups and crash reports
- Levels are backed up whenever the game saves them to `/sdcard/Benno111GDPS/LevelBackups`.
- After a native crash, the main menu offers to export a timestamped report to `/sdcard/Benno111GDPS/Crashes`.
- The app needs permission to write to shared external storage for these exports to succeed.

## Missing textures
- Missing sprite frames are replaced with a generated magenta-and-black checkerboard instead of returning a null frame that can crash the game.

## Compatibility
- Network requests advertise Geometry Dash game version `22` and binary version `46` for GDPS servers based on Geometry Dash 2.208.
- The mod save hook preserves the game's flush argument, and missing or incomplete mod save data falls back to safe defaults.
