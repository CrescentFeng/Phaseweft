#include "MotionFieldEngine.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <numbers>
#include <random>

int main() {
    constexpr auto simulatedSeconds = 45.0;
    std::mt19937 rng(0x4d463035U);
    std::uniform_real_distribution<double> bipolar(-1.0, 1.0);
    double globalPeak = 0.0;
    std::uint64_t processedFrames = 0;

    for (const auto sampleRate : {44100.0, 48000.0, 96000.0, 192000.0}) {
        motionfield::MotionFieldEngine engine;
        engine.prepare(sampleRate);
        motionfield::Parameters parameters;
        parameters.mode = motionfield::Mode::field;
        parameters.mix = 0.78;
        parameters.feedback = 0.72;
        parameters.character = 0.64;
        engine.setParameters(parameters, true);

        const auto frames = static_cast<std::uint64_t>(sampleRate * simulatedSeconds);
        for (std::uint64_t frame = 0; frame < frames; ++frame) {
            if (frame % 997U == 0U) {
                parameters.mode = static_cast<motionfield::Mode>((frame / 997U) % 4U);
                parameters.rateHz = 0.02 + 4.98 * std::abs(bipolar(rng));
                parameters.motion = 0.5 + 0.5 * bipolar(rng);
                parameters.focus = bipolar(rng);
                parameters.depth = 0.5 + 0.5 * bipolar(rng);
                parameters.spread = 0.5 + 0.5 * bipolar(rng);
                parameters.character = 0.5 + 0.5 * bipolar(rng);
                parameters.feedback = 0.88 * bipolar(rng);
                parameters.mix = 0.5 + 0.5 * bipolar(rng);
                engine.setParameters(parameters);
            }

            const auto time = static_cast<double>(frame) / sampleRate;
            const auto envelope = frame < static_cast<std::uint64_t>(sampleRate * 30.0)
                ? 0.20 : 1.0e-30;
            const auto left = envelope * (0.65 * std::sin(2.0 * std::numbers::pi * 97.0 * time)
                                        + 0.35 * bipolar(rng));
            const auto right = envelope * (0.65 * std::sin(2.0 * std::numbers::pi * 101.0 * time)
                                         + 0.35 * bipolar(rng));
            const auto output = engine.process(left, right);
            if (!std::isfinite(output.left) || !std::isfinite(output.right)) {
                std::cerr << "Non-finite output at " << sampleRate << " Hz, frame " << frame << '\n';
                return EXIT_FAILURE;
            }
            globalPeak = std::max({globalPeak, std::abs(output.left), std::abs(output.right)});
            if (globalPeak > 4.0 + 1.0e-12) {
                std::cerr << "Safety bound exceeded at " << sampleRate << " Hz\n";
                return EXIT_FAILURE;
            }
        }

        engine.reset();
        for (int i = 0; i < 4096; ++i) {
            const auto output = engine.process(0.0, 0.0);
            if (output.left != 0.0 || output.right != 0.0) {
                std::cerr << "Reset did not return the engine to exact silence\n";
                return EXIT_FAILURE;
            }
        }
        processedFrames += frames;
    }

    std::cout << "Long-run stress passed: " << processedFrames
              << " stereo frames, peak " << globalPeak << '\n';
    return EXIT_SUCCESS;
}
