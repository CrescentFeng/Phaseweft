# GitHub portfolio release checklist

Target milestone: Phaseweft v0.7.0

No tag or GitHub release should be described as complete until every required item below passes. Commercial signing, notarization and trademark registration are outside this checklist.

## Repository

- [x] Standalone Git repository initialized with an intentional candidate tracked-file set.
- [x] Private all-rights-reserved terms selected and `LICENSE` added; revisit before any public release.
- [x] JUCE version, acquisition and applicable notices documented.
- [x] Retained audio and visual assets verified as project-generated and documented.
- [x] Generated bundles, caches, archives, backups and local logs removed.
- [x] Secret and absolute-path scan clean after G0.
- [x] `.gitignore` covers build output, local JUCE checkout, credentials and host caches.

## Reproducible build

- [x] Fresh clone obtains all dependencies using documented steps.
- [x] Core-only Release build and CTest pass.
- [x] macOS Standalone, VST3 and AU build and CTest pass.
- [x] Universal `x86_64;arm64` outputs inspected.
- [x] ASan/UBSan suite rerun from the release source.
- [x] GitHub Actions macOS job passes.
- [x] Windows workflow passes, or Windows claims are explicitly removed from the release.

## Compatibility and host verification

- [x] `MotionFieldState` and published parameter IDs protected by tests.
- [x] Legacy/current/malformed state tests pass.
- [x] Phaseweft VST3 completes the REAPER G3 matrix on macOS arm64.
- [x] Phaseweft AU completes the REAPER G3 matrix on macOS arm64.
- [x] Save/close/reopen parameter recall passes.
- [x] Automation, bypass, freeze and offline render pass.
- [x] Tempo, sample-rate and buffer-size changes pass within documented hardware limits.
- [x] Host delay compensation is confirmed.
- [x] Preset display-name recall limitation is fixed and regression-tested.
- [x] No unresolved Blocker remains.

## Documentation and presentation

- [x] README states verified support and limitations accurately.
- [x] G0 audit and GitHub roadmap are present.
- [x] Build commands pass when copied from the README.
- [x] Current GUI screenshot added from a reproducible source renderer.
- [x] Demo audio reviewed and linked.
- [x] Installation and troubleshooting instructions complete.
- [x] All documentation links checked.
- [x] No commercial-release, signing or platform claim exceeds the evidence.

## Release operation

- [ ] Version and changelog finalized as v0.7.0.
- [ ] Feature freeze regression passes.
- [ ] Source archive inspected for generated or private content.
- [ ] SHA-256 generated and independently checked.
- [ ] Optional binaries clearly labelled as development and unsigned/unnotarized where applicable.
- [ ] Git tag matches the tested source exactly.
- [ ] Published archive downloaded and rebuilt successfully.
