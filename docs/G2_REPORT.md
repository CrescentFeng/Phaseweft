# G2 clean-clone build and CI report

Date: 2026-09-28
Status: **IN PROGRESS**

## Clean-clone baseline

The checks below were run from a new clone of the private GitHub repository, outside
the original working tree.

- README core-only configure/build/test path: pass, 3/3 tests.
- README default online JUCE path: pass.
- JUCE 9.0.2 archive SHA-256 verification: pass during CMake FetchContent setup.
- macOS arm64 Standalone, VST3 and AU build: pass.
- macOS arm64 full CTest suite: pass, 4/4 tests.
- ASan/UBSan core suite: pass, 3/3 tests.

## Universal macOS verification

The clean clone reused the hash-verified JUCE source obtained by the default online
path and configured the `macos-universal-release` preset.

- Universal build: pass.
- Universal CTest suite: pass, 4/4 tests.
- Standalone executable: `x86_64 arm64`.
- VST3 executable: `x86_64 arm64`.
- AU executable: `x86_64 arm64`.
- Plug-in state-test executable: `x86_64 arm64`.

## GitHub Actions diagnosis

The initial private-repository workflow run built successfully on both macOS and
Windows. The macOS Universal job passed all tests. The Windows job failed only
because the Visual Studio multi-configuration generator built `Debug`, while CTest
was invoked without a configuration and therefore reported every test as `Not Run`.

`CMakePresets.json` now explicitly selects `Release` for the Windows build and test
presets. A passing workflow on the corrected revision is still required before G2
can be marked complete.

## Remaining G2 gate

- Push the corrected preset and documentation revision.
- Confirm both macOS and Windows GitHub Actions jobs pass.
- Update this report, the roadmap, README and release checklist with the verified
  result.
