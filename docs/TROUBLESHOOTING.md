# Troubleshooting

## CMake cannot obtain JUCE

The default build needs network access to the pinned JUCE 9.0.2 archive. In an offline environment, obtain that exact JUCE source separately and configure with:

```bash
cmake -S . -B build/release \
  -DMOTION_FIELD_BUILD_PLUGIN=ON \
  -DMOTION_FIELD_JUCE_DIR=/absolute/path/to/JUCE
```

The directory must contain JUCE's top-level `CMakeLists.txt`. The repository deliberately rejects an invalid override. The pinned archive URL may be redirected to an internal mirror with `MOTION_FIELD_JUCE_URL`, but the recorded SHA-256 check remains mandatory.

## Only the DSP tests are needed

Disable plug-in targets to avoid the JUCE dependency:

```bash
cmake -S . -B build/core -DCMAKE_BUILD_TYPE=Release -DMOTION_FIELD_BUILD_PLUGIN=OFF
cmake --build build/core --parallel
ctest --test-dir build/core --output-on-failure
```

## A multi-configuration build reports missing tests

Visual Studio and some Xcode configurations need an explicit configuration for both build and CTest:

```bash
cmake --build build/release --config Release --parallel
ctest --test-dir build/release -C Release --output-on-failure
```

## The DAW does not list Phaseweft

Confirm that the complete bundle, not only its inner executable, is in a directory scanned by the DAW. Quit and reopen the host after copying it, then run the host's clear-cache/full-rescan operation. On macOS, VST3 and AU are separate formats and can appear in different browser categories. Phaseweft is an audio effect, not an instrument.

For Audio Unit diagnosis, run:

```bash
auval -v aufx Pwf1 Crsc
```

The identifiers are case-sensitive. Current Apple `auval` passed in G3. If the host continues to show an older local build, quit the host, remove only that old Phaseweft component from the user plug-in folder, install the newly built component and rescan.

## A project displays `Custom state`

This is expected when any parameter differs from all 18 compiled-in presets. v0.7 reconstructs a factory preset name only after an exact full-state match. Audio and parameter recall do not depend on the menu label.

## A legacy project uses `MotionFieldState`

That state root is intentionally retained for compatibility. Do not edit it to `PhaseweftState`. The migration layer accepts older schemas and supplies canonical defaults for parameters introduced later.

## Tempo sync does not follow the expected rate

Confirm Sync is enabled and inspect the selected Division. If the host does not provide tempo information, Phaseweft falls back to 120 BPM. Restart on Play changes modulation phase at the next stopped-to-playing transition; Free Run does not.

## Reporting a reproducible defect

Include the operating system, CPU architecture, DAW and version, plug-in format, sample rate, buffer size, build commit, minimal reproduction steps, and whether a new empty project behaves the same way. Do not attach private projects or recordings unless you have permission to share them. See [CONTRIBUTING.md](../CONTRIBUTING.md).
