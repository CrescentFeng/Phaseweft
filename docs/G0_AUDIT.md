# G0 repository audit

Date: 2026-09-27
Result: **PASS**

## Objective

Phaseweft is now scoped as a complete, publicly presentable, source-buildable and host-verified GitHub audio plug-in project. It is not currently scoped as a commercial release.

## Inventory result

The working project retains:

- Dependency-free DSP source and JUCE plug-in source.
- Core, preset, long-run, state, benchmark and demo-render targets.
- CMake presets and macOS/Windows build scripts.
- GitHub Actions build/test definition.
- One non-duplicated demonstration-audio set.
- Project, UX, preset, QA, DAW and validation documentation.

The initial audit removed from the working project:

- Generated VST3, AU and Standalone bundles.
- v0.4, v0.5 and v0.6 private/internal ZIP archives and checksums.
- Duplicate demonstration audio and REAPER peak cache.
- Historical v0.5 plug-in bundles, REAPER projects, renders and backups.
- Obsolete v0.3–v0.5 validation logs and summaries.
- Raw v0.6 logs containing local usernames or absolute filesystem paths.
- Private-beta tester material and the commercial signing/notarization helper.
- The old notice that explicitly prohibited publishing this directory.

Removed material was placed in a recoverable local Trash archive during the audit and is not part of the repository.

## Verification after cleanup

A fresh out-of-tree Release configuration was created from the cleaned desktop source using JUCE 9.0.2. Standalone, VST3, Audio Unit, core utilities and plug-in state tests built successfully.

CTest result: **4/4 passed**:

- `motion_field_core`
- `motion_field_presets`
- `motion_field_long_run`
- `motion_field_plugin_state`

## Support truth table

| Claim | G0 classification |
|---|---|
| macOS source build | Verified after cleanup |
| Core/preset/long-run/state tests | Verified after cleanup |
| v0.6 VST3/AU static validation | Verified historical summary |
| Universal `x86_64;arm64` build | Previously verified; repeat in G2 |
| Phaseweft REAPER VST3/AU workflow | Unverified; required in G3 |
| Windows build/runtime | Unverified; CI definition alone is not a pass |
| Signed/notarized public binary | Not provided and not required for G0–G5 |

## Compatibility contract retained

- The internal state root remains `MotionFieldState`.
- Existing parameter IDs remain immutable.
- State schema changes must migrate forward without overwriting valid legacy values.
- Foreign or malformed state must be ignored safely.
- The automated plug-in-state suite protects the root and expected parameter-ID set.

## Open G1 decisions

1. Choose a repository licence.
2. Replace the remaining provisional private-beta wording in the plug-in UI before screenshots are produced.

JUCE dependency acquisition, asset provenance and Git initialization moved forward in G1 after this audit was completed.

G0 establishes a clean and truthful baseline. It does not claim completion of clean-clone CI, host verification or the GitHub release.
