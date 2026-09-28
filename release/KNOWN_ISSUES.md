# Known issues — GitHub portfolio baseline

## Release blockers

- Phaseweft has not completed the G3 REAPER VST3/AU host matrix under its current identity.

## Functional and validation issues

- After host-project recall, the factory preset browser shows its neutral prompt instead of the source preset name. Parameter values are recalled correctly; no audio state is lost.
- Host automation write/read, bypass/freeze and live device/sample-rate transitions remain unverified under the Phaseweft identity.
- Host delay compensation must be confirmed in G3. The DSP impulse test and AU validation support the intended 4 ms latency after preparation, but the DAW result is the release gate.
- Windows Release build and automated tests pass in GitHub Actions. Windows DAW
  execution and Intel-host execution have not been directly observed.

## Presentation and distribution limitations

- The repository currently uses private, all-rights-reserved terms. The licence must be
  reviewed before any future change to public visibility.
- The UI still displays `PRIVATE BETA 0.6`; this must be replaced before G4 screenshots.
- The Phaseweft and Crescent Audio names are provisional and are not represented as trademark-cleared.
- No signed or notarized public binary is supplied. Signing/notarization is optional future commercial work, not a source-project blocker.
- Raw validation logs containing local filesystem paths are intentionally excluded; sanitized summaries are retained.
