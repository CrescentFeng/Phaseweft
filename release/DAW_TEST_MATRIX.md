# DAW test matrix

Status values: **Pass**, **Fail**, **In progress**, **Blocked**, **Not run**. Record host/OS versions and sanitized evidence where practical.

| Host / format | Platform | Load & UI | Playback | Automation | Offline bounce | Recall | Status |
|---|---|---:|---:|---:|---:|---:|---|
| Phaseweft / REAPER / VST3 | macOS arm64 | — | — | — | — | — | Not run |
| Phaseweft / REAPER / AU | macOS arm64 | — | — | — | — | — | Not run |
| Phaseweft / REAPER / VST3 | macOS Intel | — | — | — | — | — | Not run |
| Phaseweft / REAPER / VST3 | Windows x64 | — | — | — | — | — | Not run |

Required G3 audio configurations where supported: 44.1, 48, 96 and 192 kHz; buffer sizes 32, 64, 127, 256, 512 and 1024; mono and stereo tracks.

Required G3 workflows: fresh scan, insert/remove repeatedly, resize/reopen UI, preset loading, A/B and Undo/Redo, automation write/read, tempo and time-signature changes while synced, stopped/playing transitions, bypass, freeze, real-time and offline bounce, save/close/reopen, audio-device change and system sleep/wake.

## Historical context

A v0.5 predecessor-identity REAPER VST3 run previously passed loading, playback, save/close/reopen parameter recall and an eight-second 48 kHz/24-bit offline bounce. It did not cover automation and does not satisfy the current Phaseweft G3 rows. Its generated host project and render were removed from the GitHub working tree during G0 cleanup.
