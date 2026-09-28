# G4 documentation and demonstration report

Date: 2026-09-28

Status: **complete**

## Repository-scope cleanup

The tracked Phaseweft tree was checked for unrelated JPG/JPEG files and local absolute paths. None is tracked or staged. A generated REAPER peak cache, `artifacts/audio/peaks/00_dry.wav.reapeaks`, existed only as an ignored file in the temporary development copy and was removed.

Ignored CMake build directories remain outside version control because they are needed to verify the current changes. They are not release contents.

## Documentation completed

- Added `ARCHITECTURE.md` for the signal path, timing, permanent parameter IDs and `MotionFieldState` compatibility contract.
- Added `INSTALLATION.md` and `TROUBLESHOOTING.md` for clean source builds, local plug-in installation, DAW discovery and state/timing diagnosis.
- Added `DEMO_AUDIO.md` and README playback links for the deterministic engine and featured-preset renders.
- Added a deterministic v0.7 editor snapshot and its repository-native renderer.
- Updated the preset, product, UX, provenance, known-issues and QA documents to reflect v0.7 behaviour rather than older v0.2/v0.6 wording.
- Kept the private all-rights-reserved licence, provisional naming, unsigned/unnotarized status and unverified Windows DAW status explicit.

## Demonstration audit

The retained set contains a dry reference, four same-parameter engine comparisons and six distinct factory-preset renders. All 11 files are 48 kHz stereo, 16-bit PCM and eight seconds long. Regenerating the complete set with `motion_field_demo` produced a byte-for-byte match.

The fourth engine file was renamed from `04_motion_field.wav` to `04_field_matrix.wav`; its audio hash remains `544611be93564cffba6e24a76d6d88c84665877ca0f4503485cf1fa408c1ac4a`.

## Verification

- The complete Release build succeeded for the DSP tools, Standalone, VST3 and AU targets.
- CTest passed 4/4: core, preset audit, long-run stress and plug-in state.
- Regenerated demonstration audio matched all 11 retained files byte-for-byte.
- `motion_field_ui_snapshot` rendered the 1040×650 editor twice with an identical SHA-256: `4c2838c5d73297239a68eed5231c5fe2332535a71c0584939b033227938d0c12`.
- Every current relative Markdown link resolves to a repository file.

## Interface evidence

`artifacts/screenshots/phaseweft-v0.7-editor.png` is rendered directly from the current `MotionFieldEditor` at its documented default size, with `Utility / Mix Air Motion` loaded. It is not a desktop capture, external image or mock-up. The renderer is `tests/RenderUISnapshot.cpp` and is built as `motion_field_ui_snapshot` when plug-in targets are enabled.

G4 satisfies its exit criterion: a visitor can understand the signal design, build and install the project, audition the deterministic examples, inspect the current interface and follow the verified support boundary without private context.
