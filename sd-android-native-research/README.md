# Sleeping Dogs Android Native Port — Research Starter

This is a **native Android/ARM64 rendering prototype**, not a port of Sleeping Dogs and not affiliated with or endorsed by its rights holders.

## First milestone
- Android APK hosting a C++ renderer through JNI.
- OpenGL ES 3.0 initialization and diagnostic logs.
- A basic native render loop and touch events.
- GitHub Actions build workflow for a debug APK.

## Legal and technical boundary
This repository contains no Sleeping Dogs executable, assets, proprietary source, or decompiled code. The public TheoryEngine repository is archived and has no license; do not copy its code unless permission or qualified legal guidance is obtained.

## Build
This starter uses Android Gradle Plugin, CMake and the Android NDK. The workflow will build a debug APK. Review the build workflow and Gradle/SDK versions before running.

## Target
- Android 7.0+ (API 24+), ARM64-v8a
- OpenGL ES 3.0
- POCO X7 5G / Dimensity 7300 Ultra / Mali GPU

## Limitations
This is only a renderer smoke test. It does not load Sleeping Dogs assets or implement gameplay. A successful APK build does not prove the proprietary game engine is portable.

## Roadmap
1. Audit public engine research projects and licensing.
2. Establish a reproducible Android native build.
3. Validate rendering and input on the target device.
4. Investigate asset formats using legally accessible test data.
5. Port subsystems incrementally only where code rights and technical feasibility are established.
