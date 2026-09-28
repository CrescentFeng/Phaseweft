# G4 documentation and demonstration report

Date: 2026-09-28

Status: **in progress — one required screenshot remains**

## Repository-scope cleanup

The tracked Phaseweft tree was checked for unrelated JPG/JPEG files and local absolute paths. None is tracked or staged. A generated REAPER peak cache, `artifacts/audio/peaks/00_dry.wav.reapeaks`, existed only as an ignored file in the temporary development copy and was removed.

Ignored CMake build directories remain outside version control because they are needed to verify the current changes. They are not release contents.

## Documentation completed

- Added `ARCHITECTURE.md` for the signal path, timing, permanent parameter IDs and `MotionFieldState` compatibility contract.
- Added `INSTALLATION.md` and `TROUBLESHOOTING.md` for clean source builds, local plug-in installation, DAW discovery and state/timing diagnosis.
- Added `DEMO_AUDIO.md` and README playback links for the deterministic engine and featured-preset renders.
- Updated the preset, product, UX, provenance, known-issues and QA documents to reflect v0.7 behaviour rather than older v0.2/v0.6 wording.
- Kept the private all-rights-reserved licence, provisional naming, unsigned/unnotarized status and unverified Windows DAW status explicit.

## Demonstration audit

The retained set contains a dry reference, four same-parameter engine comparisons and six distinct factory-preset renders. All 11 files are 48 kHz stereo, 16-bit PCM and eight seconds long. Regenerating the complete set with `motion_field_demo` produced a byte-for-byte match.

The fourth engine file was renamed from `04_motion_field.wav` to `04_field_matrix.wav`; its audio hash remains `544611be93564cffba6e24a76d6d88c84665877ca0f4503485cf1fa408c1ac4a`.

## Verification

- The complete Release build succeeded for the DSP tools, Standalone, VST3 and AU targets.
- CTest passed 4/4: core, preset audit, long-run stress and plug-in state.
- Regenerated demonstration audio matched all 11 retained files byte-for-byte.
- Every current relative Markdown link resolves to a repository file. The link check will be repeated after the final screenshot is added.

## Open item

A real v0.7 source-built Standalone screenshot is still required. No unrelated local image, synthetic mock-up or screenshot from an obsolete 0.6 build will be committed as a substitute. G4 remains in progress until the correct project image is available and the final documentation-link check passes.
