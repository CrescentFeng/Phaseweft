# Changelog

## 0.7.0 — 2026-09-28 (GitHub portfolio release candidate)

- Reframed the project from a commercial private-beta candidate to a source-buildable, host-verified GitHub portfolio project.
- Removed generated binaries, duplicate audio, old private-beta packages, obsolete host artifacts and raw logs containing local paths from the working tree.
- Added a G0 repository audit, GitHub-oriented roadmap, public-release checklist, `.gitignore` and `.gitattributes`.
- Centralized and regression-tested the permanent `MotionFieldState` root plus the existing parameter-ID contract.
- Added a hash-pinned JUCE 9.0.2 FetchContent path with an explicit local-checkout override.
- Documented third-party licensing, reproducible asset provenance, contribution and security policies.
- Initialized Phaseweft as an independent Git repository and selected private, all-rights-reserved terms for its initial GitHub publication.
- Completed G2 clean-clone verification for the documented online JUCE flow, macOS arm64 and Universal builds, sanitizers, and macOS/Windows CI.
- Fixed Windows multi-configuration CTest selection by pinning the Windows build and test presets to `Release`.
- Advanced the project version and editor identity to the v0.7 portfolio candidate.
- Fixed factory-preset name reconstruction after host state recall, including immediate `Custom state` display for non-factory states.
- Added plug-in-state regression tests for factory-preset identity across binary state round-trips.
- Completed the Phaseweft-identity REAPER 7.67 VST3/AU G3 matrix on macOS arm64: playback, recall, automation, bypass, freeze/unfreeze, host PDC, online rendering and multi-rate offline rendering passed.
- Passed current Apple `auval`, including mono/stereo, parameter scheduling, bypass, latency and 11.025–192 kHz rendering.
- Completed G4 public-facing documentation with source-built installation and troubleshooting, DSP/state architecture, an indexed 11-file demonstration set and a reproducible current-editor PNG renderer.
- Renamed the fourth engine comparison from `04_motion_field.wav` to `04_field_matrix.wav` to match the current public engine label; the audio content is unchanged.
- Kept commercial naming, signing, notarization, pricing and installer work outside the current completion definition.

## 0.6.0 — 2026-09-27 (Phaseweft rebrand candidate)

- Selected **Phaseweft** as the product name after rejecting the conflicted Motion Field working name.
- Changed public VST3/AU/Standalone product metadata, bundle identifier and AU subtype to the Phaseweft private-beta identity.
- Renamed the public adaptive engine label from Motion Field to **Field Matrix** while keeping its DSP enum/index stable.
- Retained every host parameter ID and the legacy `MotionFieldState` root so binary state migration remains compatible at the parameter-state level.
- Updated the editor, accessibility text, validation/signing scripts and current product documentation.
- Passed the universal macOS Release suite (4/4), VST3 and AU pluginval strictness 10, Apple `auval`, Standalone UI smoke testing and packaged-binary signature/architecture verification.
- Added a versioned Phaseweft v0.6 private-beta archive and SHA-256 checksum. Those generated archives were later removed from the GitHub working tree during G0 cleanup.
- Kept the manufacturer name and formal trademark clearance provisional; v0.6 remains unauthorized for public sale.

## 0.5.0 — 2026-09-26 (private-beta candidate)

- Added schema-versioned plug-in state migration using canonical factory defaults for parameters absent from older sessions.
- Added plug-in state tests for legacy states, current binary round-trips and malformed/foreign state rejection.
- Added 45-second multi-rate automation and near-denormal stress tests at 44.1, 48, 96 and 192 kHz.
- Passed Release CTest (4/4) and AddressSanitizer/UndefinedBehaviorSanitizer CTest (3/3).
- Revalidated universal VST3 and AU builds with pluginval strictness 10; the installed AU also passed Apple `auval`.
- Verified REAPER 7.67 VST3 loading, playback, save/close/reopen parameter recall and 48 kHz/24-bit offline rendering.
- Added private-beta instructions, feedback template, privacy statement, known limitations and a preliminary replacement-name shortlist.
- Kept product/manufacturer names and plug-in identifiers provisional; this build is not authorized for public distribution.

## 0.4.0 — 2026-09-26 (internal release candidate)

- Added reproducible CMake presets plus macOS and Windows release build scripts.
- Added a macOS/Windows GitHub Actions build-and-test definition for later private-repository use.
- Marked the AU as sandbox-safe, enabled hardened-runtime build metadata and assigned VST3 modulation/stereo categories.
- Passed pluginval strictness 10 for VST3 across 44.1–192 kHz and block sizes 1–1024.
- Added installed-AU validation, signing/notarization handoff, DAW matrix, release checklist and name-clearance records.
- Kept all product/manufacturer names and plug-in identifiers provisional; no public release is authorized by this milestone.

## 0.3.0 — 2026-09-26

- Rebuilt the editor as a resizable 1.6:1 commercial UX prototype.
- Added an animated, accessible three-band trajectory display driven by the actual effective modulation rate.
- Added grouped Undo/Redo, previous/next preset navigation and two-slot A/B state comparison.
- Added custom rotary rendering, tooltips, control descriptions and direct value-entry text boxes.
- Completed an automated macOS accessibility-tree interaction smoke test for preset, Undo/Redo and A/B recall.

## 0.2.0 — 2026-09-26

- Added host-BPM sync with 12 straight, dotted and triplet cycle lengths.
- Added Free Run and Restart on Play transport behaviours.
- Added 18 categorized, compiled-in factory presets with host-notified parameter recall.
- Added preset stability and tempo-mapping tests.
- Added a required peak/RMS/DC audit for every factory preset.
- Corrected feedback colouring to unity small-signal gain, removing unintended self-bias at high Character/Feedback settings.
- Added six featured preset audio renders.

## 0.1.0 — 2026-09-26

- Created an independent commercial-product prototype from the ADC Flanger engineering baseline.
- Added Classic, Through-Zero, Transient Split and Motion Field modes.
- Added spectral/transient content analysis, cubic interpolation, stereo cross-feedback and feedback colour.
- Added host latency reporting, parameter smoothing and click-smoothed engine changes.
- Added JUCE VST3/AU/Standalone targets, automated tests, demo renderer and benchmark.
