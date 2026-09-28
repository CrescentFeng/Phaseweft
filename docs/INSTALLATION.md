# Build and local installation

Phaseweft v0.7.0 is a source project. The repository does not currently distribute a signed or notarized installer. Build products are development binaries for evaluation on a machine you control.

## Build from a clean clone

Requirements are CMake 3.22 or newer, Git, a C++20 compiler, and Xcode command-line tools on macOS. On Windows, use a current Visual Studio C++ toolchain.

```bash
git clone https://github.com/CrescentFeng/Phaseweft.git
cd Phaseweft
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DMOTION_FIELD_BUILD_PLUGIN=ON
cmake --build build/release --config Release --parallel
ctest --test-dir build/release -C Release --output-on-failure
```

The default configure step downloads the pinned JUCE 9.0.2 source archive and verifies its SHA-256. For a local checkout, add `-DMOTION_FIELD_JUCE_DIR=/absolute/path/to/JUCE`. An invalid path fails configuration rather than falling back silently.

Build locations vary by generator. Search below `build/release/Source/Plugin/MotionField_artefacts` for `Phaseweft.app`, `Phaseweft.vst3` and, on macOS, `Phaseweft.component`.

## macOS user-level installation

Quit the DAW before replacing a bundle. Copy the built bundles into the current user's plug-in directories:

```bash
mkdir -p "$HOME/Library/Audio/Plug-Ins/VST3" "$HOME/Library/Audio/Plug-Ins/Components"
cp -R /path/to/Phaseweft.vst3 "$HOME/Library/Audio/Plug-Ins/VST3/"
cp -R /path/to/Phaseweft.component "$HOME/Library/Audio/Plug-Ins/Components/"
```

Restart the DAW and request a plug-in rescan. The Standalone application can run directly from its build output. The locally built bundles are not Apple-notarized; macOS may apply its normal security policy according to how the files were created or transferred. Do not disable system-wide security controls. If a locally built copy is rejected, rebuild it from the trusted source checkout and inspect the exact Gatekeeper or signature message.

## Windows user-level installation

The Windows build produces VST3 and Standalone targets. Copy the built `Phaseweft.vst3` bundle to the VST3 directory configured in the DAW; the conventional system location is `C:\Program Files\Common Files\VST3`. Writing there may require administrator permission. Then rescan plug-ins in the DAW.

Windows Release compilation and automated tests pass in GitHub Actions. Windows DAW execution is not currently claimed as verified.

## Removing a local build

Quit the DAW and remove only the Phaseweft bundle you previously copied from the relevant user plug-in folder. A source-built installation does not install a service, background process or system extension.

See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for configuration, discovery and state-recall checks.
