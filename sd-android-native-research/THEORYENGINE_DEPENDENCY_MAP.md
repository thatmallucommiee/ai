# TheoryEngine Dependency Map — Android Port Blockers

Reviewed public tree and main umbrella header on 2026-10-19.
Reference: https://github.com/SDmodding/TheoryEngine

## Summary

The repository is a research reference, not a drop-in Android engine. The umbrella header hard-codes Windows/PC platform defines and includes Direct3D 11. Several platform-specific headers are only included under THEORY_PLATFORM_PC, but the umbrella itself defines that symbol unconditionally. It also advertises x64 as its target. The repo has no license and explicitly warns that some reimplementation code may resemble proprietary code. This report does not copy its implementation.

## Dependency map

| Area | Evidence in public tree | Android impact | Priority |
|---|---|---|---|
| Build/architecture | theory.hh requires C++17 and says 64-bit x64 | ARM64 must be validated; x64 assumptions may exist in types, intrinsics, packing and pointer-size logic | Blocker |
| Platform selection | theory.hh unconditionally defines THEORY_WINDOWS and THEORY_PLATFORM_PC | Must replace with a clean platform abstraction; don't merely undefine after includes | Blocker |
| Graphics | theory.hh includes d3d11.h and Illusion PC model/texture platform headers | D3D11 cannot be used as Android's native renderer; renderer/backend replacement or major port required | Blocker |
| File I/O | quark/pc/fileplat.hh and other quark/pc files | Replace Win32 file/process/timer/string assumptions with Android/POSIX implementations or an abstraction | High |
| Model/texture platform | illusion/pc/modelplat.hh and textureplat.hh | Separate file/asset parsing from GPU resource creation; implement Android GPU upload path | High |
| Engine resource/streaming | streamer/bigfile.hh, datastreamer.hh, streamresourceloader.hh, resourcefile and resource classes | Potentially useful concepts, but requires independent build and asset-format validation | High |
| Physics/animation | contrib/havok tree, physics/streaming.hh, animation/riginventory.hh | Must audit API and license/runtime requirements; no evidence yet that required runtime is Android-ready | Blocker/unknown |
| Main loop/UI/game systems | main/mainloop.hh, actiontree, FX, UEL, propertyset, NIS | Game logic is not sufficient on its own; depends on platform services, assets, rendering and animation | High |
| Code reuse rights | README says no license; some code may be identical/similar to proprietary code | Do not integrate/redistribute until rights are resolved | Blocker |

## First build experiment (do not import TheoryEngine code)

Use the existing independent Android NDK project to prove:
1. Gradle/NDK builds for arm64-v8a.
2. JNI can read a harmless app-private test file.
3. OpenGL ES can render a triangle and upload a test texture.
4. Input and lifecycle survive pause/resume.

This only validates our Android platform shell; it does not prove TheoryEngine or Sleeping Dogs compatibility.

## What must happen before a real game asset test

1. Obtain written permission or qualified legal guidance for any intended TheoryEngine reuse.
2. Work from a legally obtained copy of the game, keep proprietary files private, and never put game files in the public repo or CI artifact.
3. Identify a specific asset format and document a lawful inspection path.
4. Implement an independently authored parser/test loader only after format and rights review.
5. Demonstrate a real model/texture load on Android before expanding to streaming, animation, physics, sound, UI, missions and gameplay.

## Verdict

The dependency map finds multiple structural blockers, not one missing Android flag. A genuine native port remains unproven and is likely a substantial engine-porting/reimplementation effort. Do not describe the existing smoke-test APK as a game port.
