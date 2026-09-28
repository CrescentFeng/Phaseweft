# G1 repository and dependency report

Date: 2026-09-28
Status: **PASS**

## Git repository

- An independent Git repository was initialized on branch `main`.
- The intentional tracked-file set was committed as `128beab` and pushed to the private
  `CrescentFeng/Phaseweft` GitHub repository.
- The repository remains private and has no release tag.
- Build trees, generated plug-ins, packages, JUCE checkouts, DAW caches and common credentials are ignored.
- The repository-wide secret and absolute-local-path scan is clean.

## JUCE dependency

JUCE is not vendored in the repository.

The build supports two paths:

1. Default: CMake downloads the official JUCE 9.0.2 tag archive and verifies SHA-256 `16d01c27e8327f3644306cc5c223aa6e21af1cc1ec62faa709fc0dced57d208a`.
2. Override: `MOTION_FIELD_JUCE_DIR` points to an existing local JUCE checkout. An invalid explicit path is a configuration error.

The URL may be overridden with `MOTION_FIELD_JUCE_URL` for an offline mirror, but the pinned hash remains mandatory.

Verification:

- Local-checkout configuration: pass.
- Hash-verified archive configuration: pass.
- Full Standalone/VST3/AU build from the extracted archive: pass.
- CTest: 4/4 pass.
- Live archive transfer from the G1 working tree was interrupted by a partial network
  transfer. The later online clean-clone verification passed in G2; see
  `docs/G2_REPORT.md`.

## Asset publication

- All 11 WAV files are generated from synthetic source material by `tests/RenderDemo.cpp`.
- A fresh regeneration compared byte-for-byte equal with the retained files.
- The signal-flow SVG contains only repository-authored vectors, text and geometric primitives.
- The plug-in UI embeds no third-party image, icon or font asset.
- Provenance rules are recorded in `docs/ASSET_PROVENANCE.md`.

## Licensing boundary

JUCE 9.0.2 declares `AGPL-3.0-only OR LicenseRef-JUCE-Commercial`. Phaseweft is currently configured as a private, all-rights-reserved repository. Authorized builders must independently comply with an applicable JUCE licensing option.

The owner may manually replace the Phaseweft `LICENSE` before a future public release. AGPL-3.0-only remains the documented recommendation if the project later becomes open source.

## G1 conclusion

- Private repository rights are explicit.
- JUCE acquisition and licensing boundaries are documented.
- Retained assets have reproducible provenance.
- The independent Git repository and candidate tracked-file set are clean.

G1 is complete. G2 clean-clone and CI verification was the next gate and has since
passed; see `docs/G2_REPORT.md`.
