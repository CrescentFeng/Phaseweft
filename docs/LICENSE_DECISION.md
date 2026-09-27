# Source licence decision

Status: **selected for the private repository**

## Current choice: private, all rights reserved

The owner selected a private GitHub repository for the current development stage. The Phaseweft-authored source and project-generated assets therefore remain all rights reserved under the repository `LICENSE`.

Repository access does not grant general permission to copy, redistribute, publish or create derivative works. The owner may manually replace these terms before a later public release.

## Future public-release recommendation: AGPL-3.0-only

JUCE 9.0.2 declares its framework modules as `AGPL-3.0-only OR LicenseRef-JUCE-Commercial`. For a public, source-buildable portfolio project that does not currently require closed-source binary distribution, AGPL-3.0-only is the clearest matching open-source path.

Under this choice:

- Phaseweft source and redistributed modified versions remain available under AGPL-3.0-only.
- A user building the JUCE-linked targets can use JUCE's AGPL option, subject to the complete AGPL terms.
- Public network or redistribution obligations must be evaluated from the actual use and licence text.
- The project owner can still pursue a separate future commercial version of code for which the owner retains the necessary copyrights.
- Accepting outside code contributions later may complicate relicensing unless contribution terms are defined first.

## Current private alternative

The repository may be publicly visible without granting an open-source licence. Visitors could inspect it, but would not receive general permission to copy, redistribute or modify it. Each builder would also need their own valid JUCE licensing path.

This meets “publicly viewable” but is weaker for a collaborative or reproducible GitHub project and is not the recommended portfolio choice.

This choice was applied only after the owner explicitly requested a private release. It does not select a JUCE commercial licence or alter JUCE's own terms.

Primary references:

- https://github.com/juce-framework/JUCE/blob/9.0.2/LICENSE.md
- https://juce.com/legal/juce-9-licence/
- https://github.com/juce-framework/JUCE/blob/9.0.2/JUCE.spdx.json
