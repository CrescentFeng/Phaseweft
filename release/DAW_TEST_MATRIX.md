# DAW test matrix

Status values: **Pass**, **Fail**, **In progress**, **Blocked**, **Not run**. Record host/OS versions and sanitized evidence where practical.

| Host / format | Platform | Load & UI | Playback | Automation | Offline bounce | Recall | Status |
|---|---|---:|---:|---:|---:|---:|---|
| Phaseweft 0.7 / REAPER 7.67 / VST3 | macOS 26.6.2 arm64 | Pass | Pass | Pass | Pass, 44.1–192 kHz | Pass | Pass |
| Phaseweft 0.7 / REAPER 7.67 / AU | macOS 26.6.2 arm64 | Pass | Pass | Pass | Pass, 48 kHz | Pass | Pass |
| Phaseweft / REAPER / VST3 | macOS Intel | — | — | — | — | — | Not run |
| Phaseweft / REAPER / VST3 | Windows x64 | — | — | — | — | — | Not run |

Required G3 audio configurations where supported: 44.1, 48, 96 and 192 kHz; buffer sizes 32, 64, 127, 256, 512 and 1024; mono and stereo processing.

The macOS-arm64 G3 run covered fresh scan, both formats, UI resize/reopen,
preset loading, A/B and Undo/Redo, automation write/read, synced tempo/time-signature
changes, stopped/playing transitions, bypass, freeze/unfreeze, online and offline bounce,
save/close/reopen, audio-graph restarts and host PDC. Live VST3 passed 44.1 kHz
at 32/64/127/256/512/1024 samples plus 48 and 96 kHz at 512 samples. The built-in
device rejected live 192 kHz; VST3 offline and AU validation at 192 kHz passed.
Apple `auval` also passed one-channel rendering. A forced full-system sleep/wake
cycle was not run. See `docs/G3_REPORT.md`.

## Historical context

A v0.5 predecessor-identity REAPER VST3 run previously passed loading, playback, save/close/reopen parameter recall and an eight-second 48 kHz/24-bit offline bounce. It did not cover automation and does not satisfy the current Phaseweft G3 rows. Its generated host project and render were removed from the GitHub working tree during G0 cleanup.
