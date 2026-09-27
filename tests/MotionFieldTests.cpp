#include "CubicDelayLine.h"
#include "FactoryPresets.h"
#include "MotionFieldEngine.h"
#include "SignalAnalysis.h"
#include "TempoSync.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <string_view>

namespace {

int failures = 0;

void check(bool condition, std::string_view message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void testCubicDelay() {
    motionfield::CubicDelayLine<double> delay;
    delay.prepare(16);
    delay.push(0.0);
    delay.push(1.0);
    delay.push(2.0);
    delay.push(3.0);
    check(std::abs(delay.read(1.0) - 3.0) < 1.0e-12, "integer delay read");
    check(std::abs(delay.read(1.5) - 2.5) < 1.0e-12, "fractional interpolation on ramp");
}

void testBandReconstruction() {
    motionfield::ThreeBandSplitter splitter;
    splitter.prepare(48000.0);
    std::mt19937 rng(13);
    std::uniform_real_distribution<double> audio(-1.0, 1.0);
    for (int i = 0; i < 10000; ++i) {
        const auto input = audio(rng);
        const auto bands = splitter.process(input);
        check(std::abs((bands[0] + bands[1] + bands[2]) - input) < 1.0e-12,
              "three bands must reconstruct the input");
    }
}

void testSilenceAtSupportedSampleRates() {
    for (const auto sampleRate : {44100.0, 48000.0, 96000.0, 192000.0}) {
        for (int mode = 0; mode < static_cast<int>(motionfield::Mode::count); ++mode) {
            motionfield::MotionFieldEngine engine;
            engine.prepare(sampleRate);
            motionfield::Parameters parameters;
            parameters.mode = static_cast<motionfield::Mode>(mode);
            parameters.mix = 1.0;
            engine.setParameters(parameters, true);
            for (int i = 0; i < static_cast<int>(sampleRate * 0.05); ++i) {
                const auto output = engine.process(0.0, 0.0);
                check(output.left == 0.0 && output.right == 0.0,
                      "silence must remain silence");
                check(std::isfinite(output.left) && std::isfinite(output.right),
                      "silence output must be finite");
            }
        }
    }
}

void testReportedLatency() {
    constexpr double sampleRate = 48000.0;
    motionfield::MotionFieldEngine engine;
    engine.prepare(sampleRate);
    motionfield::Parameters parameters;
    parameters.mix = 0.0;
    engine.setParameters(parameters, true);
    const auto latency = engine.latencySamples();
    for (int i = 0; i < latency + 4; ++i) {
        const auto input = i == 0 ? 1.0 : 0.0;
        const auto output = engine.process(input, input);
        const auto expected = i == latency ? 1.0 : 0.0;
        check(std::abs(output.left - expected) < 1.0e-12,
              "reported latency must match delayed dry path");
    }
}

void testModeSwitchIsSmoothed() {
    motionfield::MotionFieldEngine engine;
    engine.prepare(48000.0);
    motionfield::Parameters parameters;
    parameters.mode = motionfield::Mode::classic;
    parameters.feedback = 0.4;
    parameters.mix = 0.8;
    engine.setParameters(parameters, true);
    double previous = 0.0;
    double maximumStep = 0.0;
    for (int i = 0; i < 24000; ++i) {
        if (i == 12000) {
            parameters.mode = motionfield::Mode::field;
            engine.setParameters(parameters);
        }
        const auto input = 0.15 * std::sin(2.0 * std::numbers::pi * 220.0
                                         * static_cast<double>(i) / 48000.0);
        const auto output = engine.process(input, input).left;
        maximumStep = std::max(maximumStep, std::abs(output - previous));
        previous = output;
    }
    check(maximumStep < 0.30, "mode switching must not create a full-scale discontinuity");
}

void testTempoSyncAndFactoryPresets() {
    check(std::abs(motionfield::tempoRateHz(120.0, 5) - 2.0) < 1.0e-12,
          "quarter-note sync at 120 BPM must be 2 Hz");
    check(std::abs(motionfield::tempoRateHz(120.0, 9) - 0.5) < 1.0e-12,
          "whole-note sync at 120 BPM must be 0.5 Hz");
    check(std::abs(motionfield::tempoRateHz(120.0, 11) - 0.125) < 1.0e-12,
          "four-bar sync at 120 BPM must be 0.125 Hz");

    std::set<std::string> names;
    for (const auto& preset : motionfield::factoryPresets) {
        check(names.insert(preset.name).second, "factory preset names must be unique");
        check(preset.divisionIndex >= 0
              && preset.divisionIndex < static_cast<int>(motionfield::tempoDivisions.size()),
              "factory preset division must be valid");
        motionfield::MotionFieldEngine engine;
        engine.prepare(48000.0);
        auto parameters = preset.parameters;
        if (preset.tempoSync)
            parameters.rateHz = motionfield::tempoRateHz(127.0, preset.divisionIndex);
        engine.setParameters(parameters, true);
        for (int i = 0; i < 12000; ++i) {
            const auto input = i == 0 ? 0.5 : 0.0;
            const auto output = engine.process(input, -input);
            check(std::isfinite(output.left) && std::isfinite(output.right),
                  "every factory preset must remain finite");
        }
    }
}

void testParameterStress() {
    motionfield::MotionFieldEngine engine;
    engine.prepare(48000.0);
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> value(-2.0, 2.0);
    std::uniform_int_distribution<int> mode(-4, 8);
    for (int i = 0; i < 300000; ++i) {
        if (i % 113 == 0) {
            motionfield::Parameters parameters;
            parameters.mode = static_cast<motionfield::Mode>(mode(rng));
            parameters.rateHz = value(rng) * 10.0;
            parameters.motion = value(rng);
            parameters.focus = value(rng);
            parameters.depth = value(rng);
            parameters.spread = value(rng);
            parameters.character = value(rng);
            parameters.feedback = value(rng);
            parameters.mix = value(rng);
            engine.setParameters(parameters);
        }
        const auto output = engine.process(0.20 * value(rng), 0.20 * value(rng));
        check(std::isfinite(output.left) && std::isfinite(output.right),
              "stress output must remain finite");
        check(std::abs(output.left) <= 4.0 && std::abs(output.right) <= 4.0,
              "safety boundary must contain extreme feedback");
        if (failures > 8) return;
    }
}

} // namespace

int main() {
    testCubicDelay();
    testBandReconstruction();
    testSilenceAtSupportedSampleRates();
    testReportedLatency();
    testModeSwitchIsSmoothed();
    testTempoSyncAndFactoryPresets();
    testParameterStress();
    if (failures == 0) std::cout << "All Phaseweft tests passed\n";
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
