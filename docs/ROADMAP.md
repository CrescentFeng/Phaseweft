# GitHub portfolio roadmap

Objective: deliver Phaseweft as a complete, publicly presentable, source-buildable and host-verified GitHub audio plug-in project.

Commercial launch work is intentionally deferred. Trademark registration, pricing, copy protection, sales infrastructure, Developer ID distribution, notarization and consumer installers are future work rather than completion gates for this roadmap.

## G0 — Scope reset and repository audit

Status: **complete**

- [x] Replace the commercial-release objective with the GitHub portfolio objective.
- [x] Inventory source, tests, documents, generated binaries and historical packages.
- [x] Remove duplicate audio, generated plug-ins, old private-beta archives and obsolete host evidence from the working tree.
- [x] Remove raw validation logs containing local absolute paths.
- [x] Preserve the `MotionFieldState` root and published parameter IDs.
- [x] Record verified, partial and unverified support claims.
- [x] Rewrite the README, roadmap, QA boundary, known issues and release checklist.
- [x] Add repository ignore rules.

Exit evidence: `docs/G0_AUDIT.md`.

## G1 — Repository and dependency hygiene

Status: **complete**

- [x] Choose private, all-rights-reserved terms and add `LICENSE`.
- [x] Add third-party notices and record the JUCE 9.0.2 licence boundary.
- [x] Make JUCE acquisition reproducible without relying on a private sibling directory.
- [x] Initialize the standalone Git repository and inspect the candidate tracked-file set.
- [x] Verify that retained WAV files are reproducible from repository source and document the SVG provenance.
- [x] Add contribution and issue-reporting guidance.
- [x] Verify that no secret, certificate, local path or generated bundle is tracked.

Exit criterion: the private repository has explicit rights terms, reproducible dependency acquisition, documented assets and no accidental local artifacts. A future public release must revisit the licence decision.

## G2 — Clean-clone reproducible build and CI

Status: **complete**

- [x] Build from a fresh clone using only documented dependency steps.
- [x] Run core, preset, long-run and plug-in-state tests.
- [x] Build Standalone, VST3 and AU on macOS.
- [x] Revalidate universal `x86_64;arm64` outputs.
- [x] Exercise the GitHub Actions macOS workflow.
- [x] Pass the Windows Release build and automated test workflow.
- [x] Defer a status badge while the repository remains private.

Exit evidence: `docs/G2_REPORT.md`.

## G3 — Phaseweft v0.7 host verification

Status: **complete**

- [x] Test REAPER discovery and loading for VST3 and AU.
- [x] Test playback, stop/start, UI reopen and resize.
- [x] Test save, close, reopen and exact parameter recall.
- [x] Test automation write/read, bypass, freeze and offline render.
- [x] Test BPM, time-signature, sample-rate and buffer-size changes.
- [x] Confirm host latency compensation for VST3 and AU.
- [x] Fix the factory-preset display-name recall limitation.
- [x] Classify every finding as Blocker, Major, Minor, environment or not run.

Exit criterion: no unresolved Blocker and no unexplained project-recall, crash, audio-corruption or latency-compensation defect.

Exit evidence: `docs/G3_REPORT.md`. Live 192 kHz was limited by the built-in audio
device, while 192 kHz offline and Apple AU rendering passed. A forced full-system
sleep/wake cycle remains a documented non-blocking gap.

## G4 — Documentation and demonstration

Status: **complete**

- [x] Add a current GUI screenshot rendered from the v0.7 source build.
- [x] Review and index the 11-file deterministic demo-audio set.
- [x] Document DSP architecture, controls, presets and state compatibility.
- [x] Add installation and troubleshooting instructions for source-built plug-ins.
- [x] Publish verified performance and validation claims only.
- [x] Make unsigned/unnotarized binary limitations explicit if binaries are offered.

Exit criterion: a new visitor can understand, build, audition and evaluate the project without private context.

Exit evidence: `docs/G4_REPORT.md` and `artifacts/screenshots/phaseweft-v0.7-editor.png`.

## G5 — GitHub portfolio release

Status: **in progress**

- [x] Freeze production features for v0.7.0.
- [x] Repeat the clean-clone build and automated regression with pinned JUCE 9.0.2.
- [x] Confirm that post-G3 changes do not alter the production DSP or plug-in implementation.
- [x] Audit the tracked tree and independently rebuild the release-candidate archive.
- [ ] Publish release notes and a source archive with SHA-256.
- [x] Omit development binaries rather than distribute unsigned/unnotarized bundles.
- [ ] Create the source tag only after every required pre-publication check passes.
- [ ] Download the published archive and verify it independently.

Exit criterion: Phaseweft can accurately be described as a complete, publicly presentable, source-buildable and host-verified GitHub audio plug-in project.

Progress evidence: `docs/G5_REPORT.md`.

## Deferred commercial track

The following are preserved as possible future work but do not block G0–G5:

- Formal trademark clearance and final manufacturer identity.
- Commercial bundle/manufacturer identifier freeze.
- Developer ID, notarization and Windows code signing.
- Installers, licensing/activation, pricing, EULA and customer support.
- Commercial cross-DAW and clean-machine certification matrix.
