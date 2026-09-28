# Demonstration audio

The repository contains a deterministic 48 kHz, stereo, 16-bit, eight-second audition set. The source is synthesized entirely by `tests/RenderDemo.cpp`; no recording, sample pack or third-party music is used.

Start with the dry reference, then compare each engine. All four engine files use the same parameter values so the algorithm is the principal variable.

| Order | File | Purpose |
|---:|---|---|
| 0 | [Dry reference](../artifacts/audio/00_dry.wav) | Synthetic transients plus a sustained stereo pad |
| 1 | [Classic](../artifacts/audio/01_classic.wav) | Conventional positive-delay flanging |
| 2 | [Through-Zero](../artifacts/audio/02_through_zero.wav) | Wet trajectory crossing the latency-aligned dry reference |
| 3 | [Transient Split](../artifacts/audio/03_transient_split.wav) | Separate attack and sustain motion |
| 4 | [Field Matrix](../artifacts/audio/04_field_matrix.wav) | Three reconstructing bands with content-aware trajectories |

The featured-preset set demonstrates six distinct musical jobs rather than small variations:

- [Transient Halo](../artifacts/audio/featured-presets/01_transient_halo.wav)
- [Hi-Hat Orbit](../artifacts/audio/featured-presets/02_hi_hat_orbit.wav)
- [Zero Jet](../artifacts/audio/featured-presets/03_zero_jet.wav)
- [Pad Drift](../artifacts/audio/featured-presets/04_pad_drift.wav)
- [Arp Lattice](../artifacts/audio/featured-presets/05_arp_lattice.wav)
- [Negative Spiral](../artifacts/audio/featured-presets/06_negative_spiral.wav)

These files are comparison evidence, not loudness-matched or mastered marketing demos. Regenerate them with:

```bash
cmake -S . -B build/core -DCMAKE_BUILD_TYPE=Release -DMOTION_FIELD_BUILD_PLUGIN=OFF
cmake --build build/core --target motion_field_demo --parallel
./build/core/motion_field_demo artifacts/audio
```

The factory-preset level audit is a separate automated test and covers all 18 presets. Asset rights and reproducibility are recorded in [ASSET_PROVENANCE.md](ASSET_PROVENANCE.md).
