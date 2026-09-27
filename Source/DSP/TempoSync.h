#pragma once

#include <algorithm>
#include <array>
#include <cstddef>

namespace motionfield {

struct TempoDivision {
    const char* name;
    double beatsPerCycle;
};

inline constexpr std::array<TempoDivision, 12> tempoDivisions{{
    {"1/16", 0.25},
    {"1/8T", 1.0 / 3.0},
    {"1/8", 0.5},
    {"1/4T", 2.0 / 3.0},
    {"1/8D", 0.75},
    {"1/4", 1.0},
    {"1/2T", 4.0 / 3.0},
    {"1/4D", 1.5},
    {"1/2", 2.0},
    {"1/1", 4.0},
    {"2/1", 8.0},
    {"4/1", 16.0},
}};

[[nodiscard]] inline double tempoRateHz(double bpm, int divisionIndex) noexcept {
    const auto safeBpm = std::clamp(bpm, 20.0, 400.0);
    const auto index = static_cast<std::size_t>(
        std::clamp(divisionIndex, 0, static_cast<int>(tempoDivisions.size()) - 1));
    return safeBpm / 60.0 / tempoDivisions[index].beatsPerCycle;
}

} // namespace motionfield
