# Contributing

Phaseweft is currently being prepared as a personal GitHub portfolio project.

## Issue reports

After the repository is published, use GitHub Issues for reproducible build failures, crashes, state-recall defects and audio problems. Include:

- operating system and architecture;
- host and plug-in format;
- sample rate and buffer size;
- exact build commit;
- minimal reproduction steps;
- whether the problem occurs in Standalone.

Do not attach private sessions, copyrighted recordings, credentials or signing material.

## Code contributions

Code contributions are currently by private invitation only. Access to the repository does not grant a licence to redistribute the source. Contribution ownership and any later public-release terms must be agreed explicitly before outside code is accepted.

Once contributions open, changes will be expected to:

- preserve `MotionFieldState` and existing parameter IDs;
- avoid allocation, locks, file access and logging in the audio callback;
- include proportionate tests;
- pass formatting, Release and sanitizer checks;
- document any new external asset or dependency.

## Scope

Commercial branding, copy protection, pricing and installer work are outside the current G0–G5 roadmap.
