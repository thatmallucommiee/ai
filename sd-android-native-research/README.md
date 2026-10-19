# Sleeping Dogs Android Native Port — Research Starter

This folder contains an ARM64 Android rendering smoke test, not a port of Sleeping Dogs and not affiliated with or endorsed by its rights holders.

## Current milestone
- Android activity calls a C++ library through JNI.
- OpenGL ES 3.0 is initialized and GPU/driver details are logged.
- Native render loop and touch-event diagnostics are in place.
- GitHub Actions workflow builds a debug APK artifact.

## Legal and technical boundary
No Sleeping Dogs executable, proprietary assets, proprietary source, or decompiled code is included. The public TheoryEngine repository is an archived reverse-engineered reimplementation, not the original source, and has no clear reuse license. Do not copy its code unless permission or qualified legal guidance is obtained. See [GENUINE_PORT_FEASIBILITY.md](GENUINE_PORT_FEASIBILITY.md) for the feasibility assessment, [THEORYENGINE_ANDROID_AUDIT.md](THEORYENGINE_ANDROID_AUDIT.md) for initial findings, and [THEORYENGINE_DEPENDENCY_MAP.md](THEORYENGINE_DEPENDENCY_MAP.md) for the dependency/blocker map.

## Build
Run the **Build Android Debug APK** workflow from the Actions tab. Workflow is scoped to this directory. It downloads Gradle 8.9, installs Android SDK packages, then builds a debug APK.

## Target
- Android API 24+
- ARM64-v8a
- OpenGL ES 3.0
- POCO X7 5G / Dimensity 7300 Ultra / Mali GPU

## Known limits
This is only a renderer smoke test. It does not load Sleeping Dogs data, implement gameplay, or prove the proprietary engine can be ported. The build has not yet been verified on the target device.

## Roadmap
1. Resolve the legal/reuse status of available engine research before integrating any of it.
2. Establish access to the user's legally obtained game data without uploading it to GitHub or CI.
3. Audit asset formats and engine/platform dependencies.
4. Prove one real asset can be loaded and rendered on Android; then expand toward gameplay.

A native port is not yet demonstrated feasible. Do not treat this smoke-test APK as the game.
