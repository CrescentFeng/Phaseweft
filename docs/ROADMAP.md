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

## G1 — Public repository and dependency hygiene

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

Status: **next**

- Build from a fresh clone using only documented dependency steps.
- Run core, preset, long-run and plug-in-state tests.
- Build Standalone, VST3 and AU on macOS.
- Revalidate universal `x86_64;arm64` outputs.
- Exercise the GitHub Actions macOS workflow.
- Run or explicitly downgrade the unverified Windows workflow claim.
- Add status badges only after workflows pass in the public repository.

Exit criterion: a clean clone and CI can reproduce every claimed build and test result.

## G3 — Phaseweft v0.7 host verification

Status: **not started**

- Test REAPER discovery and loading for VST3 and AU.
- Test playback, stop/start, UI reopen and resize.
- Test save, close, reopen and exact parameter recall.
- Test automation write/read, bypass, freeze and offline render.
- Test BPM, time-signature, sample-rate and buffer-size changes.
- Confirm host latency compensation.
- Fix the factory-preset display-name recall limitation.
- Classify every finding as Blocker, Major, Minor or Suggestion.

Exit criterion: no unresolved Blocker and no unexplained project-recall, crash, audio-corruption or latency-compensation defect.

## G4 — Documentation and demonstration

Status: **not started**

- Add current GUI screenshots.
- Review and trim the demo-audio set.
- Document DSP architecture, controls, presets and state compatibility.
- Add installation and troubleshooting instructions for source-built plug-ins.
- Publish verified performance and validation claims only.
- Make unsigned/unnotarized binary limitations explicit if binaries are offered.

Exit criterion: a new visitor can understand, build, audition and evaluate the project without private context.

## G5 — GitHub portfolio release

Status: **not started**

- Freeze features for v0.7.0.
- Repeat clean-clone build, automated tests and G3 host verification.
- Audit the tracked tree and release archive.
- Publish release notes and source archive with SHA-256.
- Optionally attach clearly labelled development binaries.
- Create the source tag only after every required checklist item passes.
- Download the published archive and verify it independently.

Exit criterion: Phaseweft can accurately be described as a complete, publicly presentable, source-buildable and host-verified GitHub audio plug-in project.

## Deferred commercial track

The following are preserved as possible future work but do not block G0–G5:

- Formal trademark clearance and final manufacturer identity.
- Commercial bundle/manufacturer identifier freeze.
- Developer ID, notarization and Windows code signing.
- Installers, licensing/activation, pricing, EULA and customer support.
- Commercial cross-DAW and clean-machine certification matrix.
