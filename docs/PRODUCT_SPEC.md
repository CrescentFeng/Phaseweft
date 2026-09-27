# Project specification — v0.6

## Product promise

**A flanger that follows the internal motion of a sound instead of sweeping the whole spectrum with one LFO.**

Phaseweft must make this distinction audible within ten seconds on drums, guitar, pads and full mixes. The unique engine is not BBD colour, through-zero operation, tempo sync or cross-feedback; those are expected supporting capabilities in the current market.

## Intended users

- Electronic producers who want evolving motion without drawing automation.
- Sound designers who need complex stereo movement from a small control set.
- Mixing engineers who want to preserve drum transients while moving sustain and ambience.

## Engines

### Classic

A stable reference mode for conventional flanging and easy A/B comparison. The delay remains on one side of the latency-aligned dry reference.

### Through-Zero

The dry path is delayed by 4 ms. The wet delay moves around that reference and can become earlier or later in relative time, producing the characteristic zero-crossing cancellation.

### Transient Split

A dual-time-constant envelope detector creates a continuous transient mask. The signal is decomposed as:

```text
x_transient[n] = mask[n] · x[n]
x_sustain[n]   = (1-mask[n]) · x[n]
```

The two components are processed by separate delay voices. Transients use a shorter, faster trajectory and restrained feedback; sustain uses a wider, slower trajectory and full feedback.

### Field Matrix

Two one-pole crossover states form three bands that reconstruct exactly by subtraction:

```text
low  = LP_320(x)
mid  = LP_2800(x) - LP_320(x)
high = x - LP_2800(x)
```

Smoothed per-band energy estimates create a low-to-high spectral-position signal. Spectral position and transient activity perturb three related modulation phases. Focus weights the delay depth toward a chosen spectral region without changing the crossover filters.

## Safety requirements

- No memory allocation, locks, file access or logging from the audio callback.
- Feedback constrained to ±0.88 before smoothing.
- Nonlinear feedback colour uses a unity-slope-at-zero saturator, so Character cannot raise the feedback loop's small-signal gain above the explicit Feedback value.
- A final emergency limiter is inactive below ±4.0 and protects against non-finite/extreme state.
- Parameter changes are smoothed; mode changes crossfade over 40 ms.
- Mono input is supported and stereo output is folded correctly for mono hosts.

## Musical timing and transport

- Manual rate remains available from 0.02–5 Hz.
- Sync mode derives rate from host BPM using 12 cycle lengths: straight, triplet and dotted values from 1/16 to four bars.
- Free Run preserves phase when playback stops and starts.
- Restart on Play resets only modulation phases on the stopped→playing transition; delay and feedback histories remain warm.
- If a host does not expose tempo, sync mode falls back to 120 BPM.

## Factory content

Version 0.2 ships an initial bank of 18 compiled-in factory presets. Presets send normal host-notified parameter changes, so loading remains automation-visible and does not bypass the plug-in state model. See `PRESET_CATALOG.md`.

A preset-audit executable measures peak level, RMS change and DC bias for all factory presets on the same reference source; it is part of CTest.

## Project identity

Phaseweft is the working project name as of v0.6. It has not received formal trademark clearance and is not presented as a commercially released product. Manufacturer text and public distribution credentials remain provisional. The internal `MotionFieldState` root and existing parameter IDs remain stable so documentation or branding changes do not silently invalidate saved sessions.
