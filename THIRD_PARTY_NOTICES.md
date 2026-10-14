# Third-party notices

## JUCE

Phaseweft uses JUCE 9.0.2 for its plug-in processor, editor, VST3, Audio Unit and Standalone targets.

- Upstream: https://github.com/juce-framework/JUCE
- Pinned release: `9.0.2`
- Source acquisition: CMake FetchContent downloads the official 9.0.2 tag archive and verifies SHA-256 `16d01c27e8327f3644306cc5c223aa6e21af1cc1ec62faa709fc0dced57d208a`; an explicit local checkout may be supplied with `MOTION_FIELD_JUCE_DIR`
- JUCE declared licence: `AGPL-3.0-only OR LicenseRef-JUCE-Commercial`
- JUCE licence file: https://github.com/juce-framework/JUCE/blob/9.0.2/LICENSE.md
- JUCE 9 EULA: https://juce.com/legal/juce-9-licence/
- JUCE software bill of materials: https://github.com/juce-framework/JUCE/blob/9.0.2/JUCE.spdx.json

JUCE is not copied into this repository. CMake downloads the pinned upstream release into the local build dependency area when plug-in targets are enabled and no local checkout is provided.

JUCE itself includes third-party components. Their versions and licence information are maintained in the upstream JUCE SPDX software bill of materials linked above.

## Phaseweft dependency boundary

The dependency-free `Source/DSP` headers and their tests use only the C++ standard library. The targets under `Source/Plugin` link against JUCE and therefore require a valid JUCE licensing path.

The Phaseweft-authored source is all rights reserved. This does not grant any rights to JUCE. Every authorized builder must use JUCE under a valid JUCE licensing option. A future AGPL-3.0-only Phaseweft release could use JUCE's matching AGPL option subject to the complete licence terms; a closed-source or differently licensed distribution must establish an appropriate JUCE commercial licensing path and perform its own compliance review.

This notice summarizes dependency provenance; it is not legal advice and does not replace the cited licence texts.

## Public inspection boundary (2026-10-14)

The publication request does not itself establish which JUCE licence the author
holds. JUCE's AGPL option does not permit additional proprietary restrictions
on a combined work where the AGPL requires corresponding rights. A proprietary
publication/distribution path must satisfy the applicable JUCE commercial EULA
(including Starter eligibility where applicable). Merely omitting JUCE files
from this repository is not a substitute for evaluating the combined work.

No JUCE-linked binary is supplied as a release asset. Source build instructions
are technical documentation; they do not confer a JUCE licence or additional
Phaseweft rights. Existing release archives retain the notices they contain.
