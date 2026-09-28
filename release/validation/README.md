# Validation evidence

This directory stores sanitized validation summaries. A pass applies only to the version and configuration named in its summary.

## Tool provenance

- pluginval: Tracktion pluginval v1.0.4.
- auval: `/usr/bin/auval` supplied by macOS.
- Internal tests: CTest targets `motion_field_core`, `motion_field_presets`, `motion_field_long_run` and `motion_field_plugin_state`.

Raw logs from v0.3–v0.5 were removed as obsolete. Raw v0.6 reports were not retained in the GitHub working tree because they contained local usernames and absolute paths. The sanitized v0.6 summary remains as historical validation evidence; current G2/G3 evidence is recorded in `docs/G2_REPORT.md` and `docs/G3_REPORT.md`.
