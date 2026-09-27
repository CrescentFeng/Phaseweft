# Phaseweft v0.6.0 validation summary

Date: 2026-09-27

## Exact build under test

- Source configuration: `build/release-macos-universal-v060`
- Formats: VST3, Audio Unit and Standalone
- Architectures: universal `x86_64 arm64`
- Version: `0.6.0`
- Bundle identifier: `audio.crescent.phaseweft`
- AU identity: `aufx/Pwf1/Crsc`
- Signing: internal ad-hoc signature with hardened-runtime options; not Developer ID signed or notarized

## Results

- Release CTest: **4/4 passed**.
- VST3 pluginval v1.0.4 strictness 10: **SUCCESS**.
- Installed Audio Unit pluginval v1.0.4 strictness 10: **SUCCESS**.
- Apple `auval -v aufx Pwf1 Crsc`: **AU VALIDATION SUCCEEDED**.
- Validation grids exercised 44.1, 48, 96 and 192 kHz and block sizes 1, 16, 64, 127, 512 and 1024; Apple validation also exercised lower rates, mono/stereo layouts, latency, bypass, scheduled parameters and the custom UI.
- Standalone accessibility smoke test exposed the Phaseweft product title, Field Matrix mode and all continuous controls.
- Deep/strict code-sign verification passed for all three bundles.

The original timestamped reports identified the exact v0.6.0 binaries but contained local absolute paths. They were removed from the GitHub working tree during G0 cleanup; this sanitized summary is retained as historical evidence. G2/G3 will generate current, sanitized evidence.

## Boundaries

- The renamed Phaseweft identity has not yet completed a recorded REAPER playback/recall/bounce pass. The retained v0.5 host project is historical predecessor evidence only.
- The v0.6 migration changed public identity, metadata and labels but did not alter the DSP core. The latest ASan/UBSan result remains the v0.5 run on that same DSP implementation.
- A repository licence, reproducible clean-clone dependency flow, Phaseweft-identity REAPER pass and public CI run remain GitHub-release gates. Trademark clearance, final manufacturer identity, Developer ID signing and notarization are deferred commercial work.
