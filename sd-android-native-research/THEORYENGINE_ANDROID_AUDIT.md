# TheoryEngine Android Port Audit — Initial Findings

## Scope

Initial source-level review of the public SDmodding/TheoryEngine repository for the goal of a genuine native ARM64 Android port of Sleeping Dogs: Definitive Edition. This is a feasibility audit, not a claim that the game currently runs on Android.

## Findings confirmed from public source and project documentation

### 1. It is a reimplementation, not the original game source
The project describes itself as a work-in-progress reverse-engineered reimplementation informed by the Steam game's PDB/debug symbols. The project says it is not a complete rewrite of the engine. The original game executable and assets remain separate requirements.
Source: https://github.com/SDmodding/TheoryEngine

### 2. Platform assumptions are Windows/x64
The main theory.hh header declares a requirement for 64-bit x86, defines THEORY_WINDOWS and THEORY_PLATFORM_PC, and conditionally includes Windows Direct3D 11 (d3d11.h). Platform-specific file, process, timer, string, model and texture headers are included for PC.
Source: https://github.com/SDmodding/TheoryEngine/blob/main/theory.hh

**Impact:** It cannot be compiled as-is for Android ARM64. Platform abstraction and architecture-dependent assumptions need review. Any code relying on Windows APIs, x64-specific types/intrinsics, or D3D11 must be replaced or isolated.

### 3. Rendering is a major blocker
Android devices such as the POCO X7 5G use ARM64 and mobile graphics APIs (OpenGL ES/Vulkan). The current TheoryEngine source explicitly exposes D3D11 dependencies. A renderer port or replacement is needed; this is not a one-line build configuration change.

### 4. The code has no clear reuse license
The repository explicitly says it has no license because some code may be identical or very similar to proprietary code, and describes the work as educational/documentation/modding-oriented. The organization also says public SDK and TheoryEngine development is paused.
Sources:
- https://github.com/SDmodding/TheoryEngine
- https://github.com/SDmodding

**Impact:** Do not copy this code into a distributed Android port or build on it for redistribution without permission from the relevant rights holders/maintainers or qualified legal advice. Public visibility is not a reuse license.

### 5. The SDK is not a portable standalone game engine
The SDmodding SDK is documented as a dev-kit/plugin SDK for the Windows game, not as an Android runtime or complete source tree for the original game.
Source: https://github.com/SDmodding/SDK

## Missing evidence / not yet verified

- A complete build of TheoryEngine on any Android/NDK toolchain.
- ARM64 compatibility of dependencies such as Havok-related code, Windows-specific helper APIs, and third-party libraries.
- Availability and terms of any required proprietary SDKs or runtime components.
- Successful parsing and rendering of real game assets on Android.
- Performance, memory use, shader compatibility, streaming behavior, animation, physics, audio, input, and save/load on the target device.
- Permission to reuse any code with unclear licensing.

## Recommended technical path

1. **Resolve reuse rights first.** Ask the TheoryEngine maintainers/rightsholders for written permission and a clear license scope for Android porting and distribution.
2. **Perform a dependency inventory.** Enumerate includes and platform-specific APIs; classify portable C++ versus Windows/x64/D3D11 dependencies.
3. **Create an Android-only branch/probe.** Do not import unlicensed code. Use a clean, independently authored NDK harness to verify Android toolchain, ARM64, file access, and GLES/Vulkan rendering.
4. **Inspect legally obtained game data locally.** Keep proprietary game files out of public GitHub repositories and CI artifacts. Start with metadata/file-format identification rather than uploading entire archives.
5. **Prove one asset pipeline.** Only after rights and formats are clear, implement a small loader for a lawful test asset and render a single scene.
6. **Expand only after each gate passes.** Streaming, animation, physics, scripting, audio, input, UI, missions and gameplay are separate major subsystems.

## Honest verdict

A genuine native Android port is **not demonstrated feasible yet**. The public codebase is a useful research reference, but its Windows/x64/Direct3D assumptions and unclear reuse license are major blockers. The previously built APK is only an OpenGL ES/JNI smoke test and does not run Sleeping Dogs code or assets.
