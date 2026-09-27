# UX specification — v0.6

## Layout

- Default size: 1040×650.
- Minimum/maximum: 820×520 and 1400×880.
- Fixed 1.6:1 aspect ratio prevents collapsed control rows or excessively stretched visualizations.
- Controls remain in four-column groups, with explicit labels and editable value boxes.

## Motion display

The display presents low, mid and high control trajectories, not an FFT or oscilloscope. Depth changes excursion, Focus changes per-band emphasis, Motion changes trajectory complexity and Spread changes offset. Animation speed reads the effective DSP rate, including the host-derived rate in Sync mode.

The display is non-interactive and cannot hide or override parameter values. Its accessibility description identifies it as a visual representation of current controls.

## Preset workflow

- Categorized menu plus previous/next navigation.
- Loading a preset is one Undo transaction rather than twelve unrelated edits.
- Selecting Undo/Redo or A/B marks the menu as a custom state to avoid falsely claiming exact preset recall after state operations.

## A/B behaviour

Two in-memory slots are initialized from the current plug-in state. When switching slots, the outgoing state is saved before the destination is recalled. The active processing state is saved normally with the host; the secondary comparison slot remains intentionally session-local in v0.6.

## Accessibility and input

- Every slider, selector, toggle and visual container exposes a title and description.
- Sync disables manual Rate and enables Division, making the active timing path explicit.
- Slider text boxes support direct numeric entry in addition to drag and keyboard control.
- Buttons provide tooltips and disabled states for unavailable Undo/Redo actions.

## Remaining validation

- Formal VoiceOver traversal and keyboard-only workflow.
- Windows scaling at 100%, 125%, 150% and 200%.
- Multiple DAW editor embedding/resizing behaviour.
- Colour-contrast audit against the final brand palette.
- Usability sessions with producers before layout and terminology are frozen.
