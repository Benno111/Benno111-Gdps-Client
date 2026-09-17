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
- Every push and pull request builds `libgame.so` and uploads it as a workflow artifact.
- Pushing a tag also creates a draft GitHub release containing the compiled library.

## External backups and crash reports
- Levels are backed up whenever the game saves them to `/sdcard/Benno111GDPS/LevelBackups`.
- Native crash details are exported to `/sdcard/Benno111GDPS/Crashes/latest-crash.txt`.
- The app needs permission to write to shared external storage for these exports to succeed.
