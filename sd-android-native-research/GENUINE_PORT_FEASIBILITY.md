# Genuine Android Port Feasibility — Research Gate

Date: 2026-10-18

## What exists publicly

- **TheoryEngine** is described by its authors as a work-in-progress reverse-engineered reimplementation of the Sleeping Dogs: Definitive Edition engine, informed by debug symbols shipped with the Steam release. It is not the original source code.
- The repository is archived and explicitly has **no license**. Its README says this is for educational, documentation, and modding purposes and requests that derivative work remain open-source. Those statements do not grant a clear license to reuse the code in an Android port. Do not copy or build it into this project unless the maintainers/rightsholders grant permission or qualified legal advice establishes a lawful basis.
- The SDmodding SDK is for making plugins/mods for the Windows game; it is not a portable Android game source tree.
- Separate tools for inspecting/unpacking game archives may be available under permissive licenses, but those tools do not change the copyright status of the game data extracted with them.

References:
- https://github.com/SDmodding/TheoryEngine
- https://github.com/SDmodding/SDK
- https://github.com/SDmodding
- https://github.com/gibbed/Gibbed.SleepingDogs
- https://store.steampowered.com/eula/307690_eula_0

## What a genuine native port would require

1. A lawful code path: permission/licensing for the relevant reverse-engineered engine code, or a clean-room implementation based on independently documented behavior.
2. The user's legally obtained game data and a lawful, non-redistributive way for the Android build to read it. Do not commit game archives, executables, keys, or extracted copyrighted assets to this repository or CI artifacts.
3. ARM64 Android support for the engine's renderer, threading, file I/O, audio, input, physics, animation, scripting, streaming, and platform services.
4. A compatibility audit of required third-party libraries, shader formats, asset formats, and engine assumptions.
5. A staged device test on the target POCO X7 5G, beginning with asset parsing and a single rendered scene before attempting gameplay.

## Current verdict

**Not yet demonstrated feasible as a genuine native Android port.** Public engine research is a useful lead, but the most promising engine repository has no clear reuse license and is archived. The current APK is only a JNI/OpenGL ES smoke test and must not be described as a port.

A Windows compatibility layer running the existing PC game would be a different approach, not a native port. A separately written lookalike is also not a port.

## Next safe steps

- Ask the TheoryEngine maintainers whether they will grant explicit permission for reuse in an Android port and clarify the exact permitted scope.
- Review the game EULA and seek qualified legal advice before redistributing or adapting proprietary code/assets.
- If permission is unavailable, investigate a clean-room engine implementation without copying unlicensed code; treat this as a major reverse-engineering project with no guarantee of full compatibility.
- Keep this repository free of proprietary game files and do not promise an APK with original gameplay until a real engine/data-loading milestone is proven.
