# Sleeping Dogs Android Native Port — Research Starter

This folder contains an ARM64 Android rendering smoke test, not a port of Sleeping Dogs and not affiliated with or endorsed by its rights holders.

## Current milestone
- Android activity calls a C++ library through JNI.
- OpenGL ES 3.0 is initialized and GPU/driver details are logged.
- Native render loop and touch-event diagnostics are in place.
- GitHub Actions workflow builds a debug APK artifact.

## Legal and technical boundary
No Sleeping Dogs executable, proprietary assets, proprietary source, or decompiled code is included. The public TheoryEngine repository is archived and has no license; do not copy its code unless permission or qualified legal guidance is obtained.

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
1. Verify CI build and fix any build errors.
2. Install APK on the POCO X7 and capture renderer/GPU logs.
3. Audit engine research sources and their reuse permissions.
4. Build a clean, independently implemented Android engine layer; add original game asset support only after technical and legal review.
