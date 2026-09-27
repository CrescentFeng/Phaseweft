#include "FactoryPresets.h"
#include "MotionFieldEngine.h"
#include "TempoSync.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numbers>

namespace {

struct Metrics {
    double sumSquares{};
    double sum{};
    double peak{};
    std::size_t count{};

    void add(double left, double right) noexcept {
        sumSquares += left * left + right * right;
        sum += left + right;
        peak = std::max({peak, std::abs(left), std::abs(right)});
        count += 2U;
    }

    [[nodiscard]] double rms() const noexcept {
        return std::sqrt(sumSquares / static_cast<double>(count));
    }

    [[nodiscard]] double dc() const noexcept {
        return sum / static_cast<double>(count);
    }
};

struct StereoInput {
    double left{};
    double right{};
};

StereoInput sourceAt(std::size_t index, double sampleRate) noexcept {
    const auto time = static_cast<double>(index) / sampleRate;
    const auto beatPhase = std::fmod(time * 2.0, 1.0);
    const auto transientEnvelope = std::exp(-beatPhase * 30.0);
    const auto click = transientEnvelope * std::sin(2.0 * std::numbers::pi
                                                   * (150.0 - 90.0 * beatPhase) * time);
    const auto padLeft = 0.16 * (std::sin(2.0 * std::numbers::pi * 110.0 * time)
                               + 0.45 * std::sin(2.0 * std::numbers::pi * 277.18 * time));
    const auto padRight = 0.16 * (std::sin(2.0 * std::numbers::pi * 110.4 * time)
                                + 0.45 * std::sin(2.0 * std::numbers::pi * 329.63 * time));
    return {padLeft + 0.28 * click, padRight + 0.28 * click};
}

} // namespace

int main() {
    constexpr double sampleRate = 48000.0;
    constexpr std::size_t sampleCount = 48000U * 8U;
    Metrics inputMetrics;
    for (std::size_t i = 0; i < sampleCount; ++i) {
        const auto input = sourceAt(i, sampleRate);
        inputMetrics.add(input.left, input.right);
    }

    std::cout << "category,preset,peak_dbfs,rms_delta_db,dc\n";
    auto failed = false;
    for (const auto& preset : motionfield::factoryPresets) {
        motionfield::MotionFieldEngine engine;
        engine.prepare(sampleRate);
        auto parameters = preset.parameters;
        if (preset.tempoSync)
            parameters.rateHz = motionfield::tempoRateHz(120.0, preset.divisionIndex);
        engine.setParameters(parameters, true);

        Metrics outputMetrics;
        for (std::size_t i = 0; i < sampleCount; ++i) {
            const auto input = sourceAt(i, sampleRate);
            const auto output = engine.process(input.left, input.right);
            outputMetrics.add(output.left, output.right);
        }
        const auto peakDb = 20.0 * std::log10(std::max(outputMetrics.peak, 1.0e-12));
        const auto rmsDelta = 20.0 * std::log10(
            std::max(outputMetrics.rms(), 1.0e-12) / std::max(inputMetrics.rms(), 1.0e-12));
        std::cout << preset.category << ',' << preset.name << ','
                  << std::fixed << std::setprecision(2) << peakDb << ','
                  << rmsDelta << ',' << std::scientific << outputMetrics.dc() << '\n';
        if (!std::isfinite(peakDb) || !std::isfinite(rmsDelta)
            || outputMetrics.peak >= 1.0 || std::abs(outputMetrics.dc()) >= 1.0e-3)
            failed = true;
    }
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
