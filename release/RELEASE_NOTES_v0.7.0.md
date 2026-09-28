# Phaseweft v0.7.0

Phaseweft v0.7.0 is the source release for a complete, publicly presentable, source-buildable and REAPER-verified GitHub audio plug-in project. It is an engineering/portfolio release, not a signed commercial distribution.

## What is included

- Four engines: Classic, Through-Zero, Transient Split and Field Matrix.
- Host-tempo sync, restart-on-play modulation and 18 factory presets.
- A resizable editor with A/B comparison, Undo/Redo and exact preset-name reconstruction.
- Stable parameter IDs, versioned state migration and the retained `MotionFieldState` compatibility root.
- macOS Standalone, VST3 and Audio Unit targets; Windows VST3 and Standalone build targets.
- Deterministic audio demonstrations, a reproducible UI snapshot and documented asset provenance.
- Build, installation, troubleshooting, architecture, QA and host-verification documentation.

## Verified boundary

- Clean-clone macOS Release build and 4/4 automated tests pass with the hash-pinned JUCE 9.0.2 dependency.
- GitHub Actions macOS universal and Windows Release jobs passed during G2.
- REAPER 7.67 on macOS arm64 passed the recorded v0.7 VST3/AU G3 workflow, including recall, automation, bypass, freeze, rendering and host latency compensation.
- Current Apple `auval` passed through 192 kHz.

Windows DAW execution and Intel-host execution have not been directly observed. A forced full-system sleep/wake cycle was not run. See `docs/G3_REPORT.md` and `release/KNOWN_ISSUES.md` for the exact limits.

## Distribution and licensing

This release contains source only. No unsigned development binary, installer, code-signing identity or notarized package is attached. The repository is currently private and all rights reserved; access does not grant redistribution rights. JUCE remains subject to its own AGPL-3.0-only or commercial licence options as described in `THIRD_PARTY_NOTICES.md`.

The Phaseweft and Crescent Audio names are provisional and are not represented as trademark-cleared.

## Build

```bash
cmake -S . -B build/release \
  -DCMAKE_BUILD_TYPE=Release \
  -DMOTION_FIELD_BUILD_PLUGIN=ON
cmake --build build/release --config Release --parallel
ctest --test-dir build/release -C Release --output-on-failure
```

The default configure path downloads JUCE 9.0.2 and verifies its recorded SHA-256 before use.
