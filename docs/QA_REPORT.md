# QA report — Phaseweft v0.7.0 GitHub portfolio candidate

Date: 2026-09-28

## Automated coverage

| Test | Result |
|---|---|
| Cubic/integer fractional-delay reads | Pass |
| Exact three-band reconstruction | Pass |
| Silence preservation | Pass |
| 44.1, 48, 96 and 192 kHz processing | Pass |
| Reported 4 ms latency vs impulse | Pass |
| Smoothed Classic → Field switch | Pass |
| 300,000-frame invalid/extreme parameter stress | Pass |
| Finite and bounded output under stress | Pass |
| Tempo-rate mapping at 120 BPM | Pass |
| Factory preset names and division bounds | Pass |
| All 18 presets: finite impulse-tail render | Pass |
| 45-second automation/denormal stress at 44.1/48/96/192 kHz | Pass |
| Legacy state migration from missing v0.2 parameters | Pass |
| Current binary state round-trip | Pass |
| Factory-preset identity after binary state round-trip | Pass |
| Edited factory state becomes `Custom state` | Pass |
| Malformed and foreign state rejection | Pass |

Commands:

- `ctest --test-dir build/release-macos-universal-v060 --output-on-failure`
- `ctest --test-dir build/asan-v050 --output-on-failure`

After the G0 cleanup, a fresh out-of-tree Release configuration built the Standalone, VST3 and AU targets from the retained source and passed the complete 4/4 CTest suite. This confirms that removal of generated and historical artifacts did not damage the source project.

Release CTest result: **4/4 passed** (`motion_field_core`, `motion_field_presets`, `motion_field_long_run`, `motion_field_plugin_state`).

ASan/UBSan CTest result: **3/3 passed** (`motion_field_core`, `motion_field_presets`, `motion_field_long_run`) on the unchanged v0.5 DSP implementation. The v0.6 change is public identity/metadata and did not alter the DSP core.

## Build coverage

| Target | Result |
|---|---|
| macOS arm64 Standalone | Built |
| macOS arm64 Audio Unit | Built |
| macOS arm64 VST3 | Built |
| Windows VST3/Standalone | Release build and 4/4 tests pass in GitHub Actions; DAW use not tested |
| macOS universal `x86_64 arm64` Standalone | Built; arm64 executed |
| macOS universal `x86_64 arm64` Audio Unit | Built; arm64 validated |
| macOS universal `x86_64 arm64` VST3 | Built; arm64 validated |

## Performance snapshot

- Configuration: Release, Field Matrix mode, all engines continuously warm.
- Result: five-run median approximately 171 ns/stereo frame and 122× real time at 48 kHz.
- The measurement is local and must be repeated on the release matrix.

## Factory preset level audit

- All 18 presets remained below 0 dBFS on the eight-second reference source.
- Highest measured peak: approximately -5.9 dBFS.
- DC magnitude remained below `4e-4` for every preset.
- The audit exposed an early feedback-colour design whose makeup gain raised the loop's small-signal gain. The saturator was corrected to unity slope at zero and the audit was promoted to a required CTest target.

## Plug-in validation — Phaseweft v0.6.0

- VST3: pluginval v1.0.4, strictness 10, **SUCCESS**.
- AU: pluginval v1.0.4, strictness 10, **SUCCESS**; its embedded `auval` returned code 0.
- Apple `auval -v aufx Pwf1 Crsc`: **AU VALIDATION SUCCEEDED**.
- pluginval processing/automation grids covered 44.1, 48, 96 and 192 kHz with blocks 1, 16, 64, 127, 512 and 1024.
- Apple AU render coverage additionally included 11.025 and 22.05 kHz, mono/stereo layouts, latency, bypass, parameter scheduling and custom UI.
- Both plug-in binaries are universal `x86_64 arm64`, have sandbox-safe AU metadata and pass deep/strict code-sign verification with internal ad-hoc hardened-runtime signatures.

## REAPER host test — historical v0.5 predecessor result

- REAPER 7.67/macOS-arm64 discovered and loaded the installed VST3 at 48 kHz/512 samples.
- The editor exposed all controls and accessibility descriptions.
- `Utility / Gentle Width` values were saved with Sync enabled and Division set to `1/1`.
- After closing and reopening the project, Sync, Division and all continuous parameters were restored exactly.
- The factory preset browser returns to its neutral label after project recall even though the actual parameter state is restored; this is recorded as a UX limitation rather than a state-loss failure.
- Full-speed offline rendering produced an 8.000-second, 48 kHz, 24-bit stereo WAV. REAPER reported -6.5 dBFS peak and zero clipped samples.
- The local project/render artifacts were removed from the GitHub working tree during G0 cleanup because they belong to an obsolete identity and are not current release evidence.
- This result predates the Phaseweft identity migration and is retained only as historical context. The current-identity result below supersedes it.

## REAPER host test — Phaseweft v0.7 current identity

- REAPER 7.67 on macOS 26.6.2/arm64 discovered and loaded both installed Phaseweft VST3 and AU bundles.
- Both formats passed playback, stop/start, editor reopen, bypass, stereo freeze/playback/unfreeze and save/close/reopen recall.
- The VST3 editor also passed resize, preset navigation, Undo/Redo and A/B interaction checks.
- The factory-preset display-name limitation was fixed. `Utility / Mix Air Motion` and all eight continuous values survived VST3 and AU project recall.
- Host-native Mix envelopes played from 0.20 through 0.80 and back in both formats.
- A synced VST3 run passed 120 BPM 4/4, 90 BPM 3/4 and 150 BPM 7/8 markers.
- REAPER reported Phaseweft's 192-sample latency at 48 kHz and applied 512-sample block-aligned PDC for both formats.
- Live VST3 playback passed all required buffer sizes at 44.1 kHz and additionally passed 48/96 kHz at 512 samples.
- VST3 offline renders passed at 44.1, 48, 96 and 192 kHz. AU 48 kHz/24-bit stereo online and offline renders also passed with zero clipped samples.
- Current Apple `auval` passed mono/stereo, latency, bypass, parameter scheduling, custom UI and renders through 192 kHz.

The complete sanitized evidence and finding dispositions are in `docs/G3_REPORT.md`.

## Not yet GitHub-release-qualified

- G4 screenshots, installation/troubleshooting guidance and final demonstration review remain open.
- G5 feature-freeze regression, archive inspection, checksum, tag and independent rebuild remain open.
- Windows DAW and Intel-host execution have not been observed and must not be claimed.
- A forced full-system sleep/wake cycle was not run; audio-graph restarts and live rate changes passed.
- Signed/notarized installers are not part of the GitHub portfolio completion definition; any attached development binary must state its limitations.

## UX interaction smoke test

- Standalone opened at 1040×650 with resize limits of 820×520–1400×880.
- macOS accessibility inspection exposed descriptive names for all eight continuous controls, engine, sync, division, transport and visualization.
- Next-preset navigation loaded `Utility / Gentle Width` and enabled Undo.
- Undo restored the complete initial parameter state and enabled Redo.
- Redo restored the preset state.
- Switching A→B stored the outgoing preset state and recalled the initial comparison state; switching B→A restored the preset values.
- Tempo Division and manual Rate correctly expose mutually exclusive enabled states when Sync changes.
- The Phaseweft v0.6 Standalone opened successfully after the rebrand; accessibility inspection exposed the Phaseweft title, Field Matrix mode label and all eight continuous parameter controls.

Formal VoiceOver, keyboard-only navigation and multiple-DPI host testing remain release gates.
