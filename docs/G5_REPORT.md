# G5 portfolio release report

Date: 2026-09-28

Status: **release candidate verification in progress**

## Feature freeze boundary

Production DSP and plug-in implementation files are unchanged from the source that passed the full G3 REAPER VST3/AU matrix. Changes after G3 are documentation, demonstration naming, a generated interface image, and an `EXCLUDE_FROM_ALL` UI-snapshot utility. The new utility links the existing plug-in library but is not part of the Standalone, VST3 or AU products.

No optional binary attachment is planned. The v0.7.0 GitHub release is source-only, avoiding any implication that the ad-hoc local macOS bundles are signed or notarized distribution products.

## Clean-clone regression

A fresh local clone of the pushed G4 commit configured through the documented default dependency path. CMake downloaded the locked JUCE 9.0.2 archive and verified its SHA-256.

- Release build: pass.
- Standalone, VST3 and Audio Unit targets: pass.
- CTest: 4/4 pass (`motion_field_core`, `motion_field_presets`, `motion_field_long_run`, `motion_field_plugin_state`).
- On-demand `motion_field_ui_snapshot` build: pass.
- JUCE 9.0.2 UI snapshot vs tracked PNG: byte-identical.

The G3 host evidence remains applicable because the production DSP, processor, editor and state code are unchanged. The final feature-freeze audit records that fact instead of claiming an unrun new host matrix for documentation-only changes.

## Source archive audit

The release-candidate Git archive contains 74 tracked files under one `Phaseweft-0.7.0` directory. It contains no Git metadata, build tree, cache, log, certificate, key, environment file, nested archive or absolute local path. The checksum file contains only the archive basename.

The archive was extracted into a separate temporary directory, configured against the same verified JUCE 9.0.2 source, built as Standalone/VST3/AU and passed CTest 4/4. Bundle metadata reported version `0.7.0` and identifier `audio.crescent.phaseweft`.

Raw CMake bundles are local build products rather than release attachments. Strict signature checks can fail after generators add bundle resources. The macOS release-build script now performs and verifies an explicit local ad-hoc re-sign; this does not claim Developer ID signing or notarization.

## Publication gates

Before creating `v0.7.0`, the final tracked tree and exact source archive must be scanned, the archive must be independently rebuilt, and its SHA-256 must be recorded. The tag and private GitHub Release will not be created until those checks pass.
