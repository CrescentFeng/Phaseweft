# G3 Phaseweft v0.7 REAPER host-verification report

Date: 2026-09-28
Status: **PASS with documented environment limits**

## Scope and environment

- Product version: Phaseweft 0.7.0.
- Host: REAPER 7.67, native arm64.
- System: macOS 26.6.2 on Apple M5 Pro, 24 GB memory.
- Formats exercised in REAPER: VST3 and Audio Unit v2.
- Installed candidate: locally built arm64 bundles with ad-hoc signatures.
- Test source: the repository's eight-second stereo `artifacts/audio/00_dry.wav`.

Raw REAPER projects, renders and machine-local logs were kept outside the repository.
This document is the sanitized evidence record.

## G3 code changes

- Bumped the project version from 0.6.0 to 0.7.0.
- Replaced the private-beta editor caption with `PORTFOLIO CANDIDATE 0.7`.
- Derived the displayed factory-preset identity from the complete parameter state.
- Kept the preset name synchronized after host recall, Undo/Redo and A/B changes.
- Made a restored non-factory state display `Custom state` immediately when its editor opens.
- Added plug-in-state regression coverage for factory-preset identity after binary state round-trip and for edited custom states.

The existing `MotionFieldState` root and all published parameter IDs remain unchanged.

## Automated and format validation

- Release build: Standalone, VST3 and AU passed.
- CTest: 4/4 passed (`motion_field_core`, `motion_field_presets`,
  `motion_field_long_run`, `motion_field_plugin_state`).
- Deep/strict code-sign verification: passed for all three ad-hoc-signed bundles.
- Apple `auval -v aufx Pwf1 Crsc`: **AU VALIDATION SUCCEEDED**.
- `auval` covered mono and stereo, latency, bypass, parameter scheduling, Cocoa UI,
  11.025/22.05/44.1/48/96/192 kHz, and render blocks from 64 to 4096 samples.

## REAPER VST3 result

| Workflow | Result |
|---|---|
| Fresh rescan, discovery and insert | Pass |
| Native editor, reopen and resize | Pass |
| Playback and stopped/playing transitions | Pass |
| Factory preset navigation | Pass |
| Undo/Redo and A/B recall | Pass |
| Manual edit to `Custom state` | Pass |
| Save, close, reopen and exact parameter recall | Pass |
| Factory preset display-name recall | Pass |
| Mix envelope write/read | Pass; observed normalized range 0.200000–0.799997 |
| 120 BPM 4/4 → 90 BPM 3/4 → 150 BPM 7/8 while Sync was enabled | Pass |
| Host bypass and restore | Pass |
| Stereo freeze, playback and unfreeze | Pass |
| Full-speed offline render | Pass; 0 clipped samples |
| Host delay compensation | Pass |

The recalled `Utility / Mix Air Motion` state matched Rate 0.090, Motion 0.46,
Focus 0.72, Depth 0.24, Spread 0.30, Character 0.03, Feedback 0.08 and Mix 0.20.

## REAPER Audio Unit result

| Workflow | Result |
|---|---|
| Discovery and insert under the Phaseweft identity | Pass |
| Native Cocoa editor | Pass |
| Playback and stopped/playing transitions | Pass |
| Save, close, reopen and exact parameter recall | Pass |
| Factory preset display-name recall | Pass |
| Mix envelope write/read | Pass; observed normalized range 0.200000–0.800000 |
| Host bypass and restore | Pass |
| Stereo freeze, playback and unfreeze | Pass |
| 48 kHz/24-bit stereo online render | Pass; 1.0× real time, 0 clipped samples |
| 48 kHz/24-bit stereo offline render | Pass; 46× real time, 0 clipped samples |
| Host delay compensation | Pass |

The AU recall project reopened with `Utility / Mix Air Motion` and the same eight
continuous values listed above.

## Sample-rate and buffer coverage

Live VST3 playback passed at 44.1 kHz with block sizes 32, 64, 127, 256, 512 and
1024. Additional live graph restarts and playback passed at 48 kHz/512 and
96 kHz/512. The built-in Mac audio device did not accept a live 192 kHz request
and remained at 96 kHz; this is a hardware limit, not a plug-in failure.

VST3 full-speed offline renders passed at 44.1, 48, 96 and 192 kHz. All were
24-bit stereo WAV files with no clipped samples. The final AU render passed at
48 kHz/24-bit stereo. Apple `auval` independently passed Phaseweft at 192 kHz and
in a one-channel configuration.

## Delay-compensation evidence

At the live 48 kHz/512-sample configuration, REAPER exposed:

- `chain_pdc_reporting=192` — Phaseweft's intended 4 ms latency at 48 kHz.
- `chain_pdc_actual=512` — REAPER's processing-block-aligned compensation.

The same values were observed for VST3 and AU after playback initialized their
audio graphs. This closes the G3 host-PDC gate.

## Render evidence

| Format | Rate | Duration | SHA-256 |
|---|---:|---:|---|
| VST3 WAV, 24-bit stereo | 44.1 kHz | 7.6 s | `f43c7bf7db5a3883a5b6e64131d6dc2b9cce88fd3cf403277044746e0612831d` |
| VST3 WAV, 24-bit stereo | 48 kHz | 7.6 s | `ca07b8ce30090fd2d86ffca6ee850b4281a1e0fef0585724b98e5a78a9da65d7` |
| VST3 WAV, 24-bit stereo | 96 kHz | 7.6 s | `9b9e3fd46f021ce4ef98fea257386a086940dd3c22655477dd8e8c185f6da671` |
| VST3 WAV, 24-bit stereo | 192 kHz | 7.6 s | `d9dd3aafa6b6f430b4c17deea0e75eb70a2c47a2a4ffc53f41deea5e9c159bb4` |
| AU WAV, 24-bit stereo | 48 kHz | 8.0 s | `3e6c42bd460245e2df75230eb48163ecd7c9c5a41376c97746f98fe7373ff4b7` |
| AU online WAV, 24-bit stereo | 48 kHz | 8.0 s | `6ff29dda319c2a22f1ebaef08cdcd6b6eda073e8ad461b63070288a4a20fe6f1` |

## Findings and disposition

| ID | Severity | Finding | Disposition |
|---|---|---|---|
| G3-01 | Major | Factory preset name was not reconstructed after host state recall. | Fixed and covered by automated plus VST3/AU host recall tests. |
| G3-02 | Minor | A newly opened editor could retain the neutral prompt for an initial custom state. | Fixed with a distinct uninitialized display sentinel. |
| G3-03 | Environment | The built-in device rejected live 192 kHz and stayed at 96 kHz. | Accepted; 192 kHz offline VST3 and `auval` render tests pass. |
| G3-04 | Validation gap | The available pluginval executable aborts on this OS before producing a plug-in result. | Retain the historical v0.6 pluginval result; current `auval`, REAPER and automated results are authoritative for G3. |
| G3-05 | Not run | A full macOS sleep/wake cycle was not forced during the automated desktop session. | Non-blocking for the portfolio milestone; repeated audio-graph restarts and device-rate changes passed. |

No Blocker or unresolved Major finding remains. Windows DAW execution and Intel
hardware execution remain outside this macOS-arm64 G3 result and must not be
claimed as verified.

## Conclusion

Phaseweft 0.7.0 passes the current-identity REAPER VST3/AU host gate on the tested
macOS arm64 system. G4 documentation and demonstration work is next.
