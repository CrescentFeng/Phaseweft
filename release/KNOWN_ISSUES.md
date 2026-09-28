# Known issues — GitHub portfolio baseline

## Release blockers

- No known G3 host-verification blocker remains.

## Functional and validation issues

- The factory-preset display-name recall issue was fixed in v0.7 and passed VST3/AU save-close-reopen tests.
- REAPER VST3/AU automation, bypass, freeze/unfreeze, offline rendering and host delay compensation passed on macOS arm64.
- The built-in Mac audio device used for G3 accepts live rates through 96 kHz, not 192 kHz. VST3 offline rendering and Apple `auval` passed at 192 kHz.
- A forced full-system sleep/wake cycle was not run. Repeated audio-graph restarts, live sample-rate changes and project reopen passed.
- The available pluginval executable aborts on macOS 26.6.2 before producing a current v0.7 plug-in result. Historical v0.6 pluginval strictness-10 results remain recorded; current Apple `auval`, REAPER and automated validation pass.
- Windows Release build and automated tests pass in GitHub Actions. Windows DAW
  execution and Intel-host execution have not been directly observed.

## Presentation and distribution limitations

- The repository currently uses private, all-rights-reserved terms. The licence must be
  reviewed before any future change to public visibility.
- The UI identifies the build as `PORTFOLIO CANDIDATE 0.7`; final G4 screenshots are still required.
- The Phaseweft and Crescent Audio names are provisional and are not represented as trademark-cleared.
- No signed or notarized public binary is supplied. Signing/notarization is optional future commercial work, not a source-project blocker.
- Raw validation logs containing local filesystem paths are intentionally excluded; sanitized summaries are retained.
