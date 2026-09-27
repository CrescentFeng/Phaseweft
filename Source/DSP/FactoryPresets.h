#pragma once

#include "MotionFieldEngine.h"

#include <array>

namespace motionfield {

struct FactoryPreset {
    const char* category;
    const char* name;
    Parameters parameters;
    bool tempoSync;
    int divisionIndex;
    int transportMode;
};

inline constexpr std::array<FactoryPreset, 18> factoryPresets{{
    {"Utility", "Gentle Width", {Mode::field, 0.14, 0.35, 0.0, 0.30, 0.38, 0.04, 0.12, 0.28}, false, 9, 0},
    {"Utility", "Mix Air Motion", {Mode::field, 0.09, 0.46, 0.72, 0.24, 0.30, 0.03, 0.08, 0.20}, false, 9, 0},
    {"Utility", "Mono-Safe Slow", {Mode::throughZero, 0.07, 0.20, 0.0, 0.22, 0.06, 0.02, 0.10, 0.24}, false, 9, 0},
    {"Drums", "Transient Halo", {Mode::transient, 0.28, 0.62, 0.15, 0.58, 0.54, 0.12, 0.24, 0.42}, true, 9, 1},
    {"Drums", "Hi-Hat Orbit", {Mode::field, 0.52, 0.88, 0.88, 0.76, 0.82, 0.08, 0.20, 0.52}, true, 8, 1},
    {"Drums", "Snare Vacuum", {Mode::throughZero, 0.38, 0.40, 0.0, 0.92, 0.45, 0.18, -0.42, 0.68}, true, 9, 1},
    {"Drums", "Drum Bus Lift", {Mode::transient, 0.12, 0.42, -0.10, 0.34, 0.28, 0.05, 0.16, 0.26}, true, 10, 1},
    {"Guitar", "Zero Jet", {Mode::throughZero, 0.18, 0.30, 0.0, 0.96, 0.40, 0.22, 0.58, 0.62}, false, 9, 0},
    {"Guitar", "Clean Double", {Mode::classic, 0.31, 0.20, 0.0, 0.32, 0.48, 0.02, 0.08, 0.36}, false, 9, 0},
    {"Guitar", "Analog Sweep", {Mode::classic, 0.16, 0.28, 0.0, 0.78, 0.58, 0.64, 0.54, 0.58}, true, 10, 1},
    {"Synth", "Pad Drift", {Mode::field, 0.08, 0.82, 0.10, 0.84, 0.78, 0.20, 0.46, 0.60}, true, 11, 0},
    {"Synth", "Bass Current", {Mode::field, 0.11, 0.58, -0.82, 0.44, 0.18, 0.16, 0.26, 0.38}, true, 10, 1},
    {"Synth", "Arp Lattice", {Mode::field, 0.45, 0.94, 0.32, 0.72, 0.92, 0.08, -0.28, 0.54}, true, 7, 1},
    {"Synth", "Glass Orbit", {Mode::transient, 0.24, 0.76, 0.64, 0.66, 0.88, 0.06, 0.34, 0.50}, true, 9, 1},
    {"Experimental", "Negative Spiral", {Mode::field, 0.20, 0.92, -0.10, 0.82, 0.90, 0.38, -0.48, 0.56}, true, 8, 1},
    {"Experimental", "Field Collapse", {Mode::field, 0.05, 1.0, 0.0, 1.0, 0.12, 0.32, 0.70, 0.78}, false, 9, 0},
    {"Experimental", "Feedback Bloom", {Mode::throughZero, 0.06, 0.54, 0.0, 0.90, 0.74, 0.72, 0.78, 0.64}, true, 11, 0},
    {"Experimental", "Broken Compass", {Mode::transient, 0.63, 1.0, -0.45, 0.82, 1.0, 0.52, -0.60, 0.72}, true, 6, 1},
}};

} // namespace motionfield
