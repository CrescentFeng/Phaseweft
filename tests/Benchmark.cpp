#include "MotionFieldEngine.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>

int main() {
    constexpr double sampleRate = 48000.0;
    constexpr int sampleCount = 48000 * 30;
    motionfield::MotionFieldEngine engine;
    engine.prepare(sampleRate);
    motionfield::Parameters parameters;
    parameters.mode = motionfield::Mode::field;
    parameters.motion = 0.85;
    parameters.depth = 0.9;
    parameters.feedback = 0.55;
    engine.setParameters(parameters, true);

    double checksum = 0.0;
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < sampleCount; ++i) {
        const auto time = static_cast<double>(i) / sampleRate;
        const auto left = 0.2 * std::sin(1382.30076758 * time);
        const auto right = 0.2 * std::sin(1385.44236023 * time);
        const auto output = engine.process(left, right);
        checksum += output.left + output.right;
    }
    const auto elapsed = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - start).count();
    const auto nanosecondsPerStereoSample = elapsed * 1.0e9 / static_cast<double>(sampleCount);
    const auto realtimeFactor = (static_cast<double>(sampleCount) / sampleRate) / elapsed;
    std::cout << "Phaseweft benchmark\n"
              << "  ns/stereo-frame: " << nanosecondsPerStereoSample << '\n'
              << "  realtime factor: " << realtimeFactor << "x\n"
              << "  checksum: " << checksum << '\n';
    return std::isfinite(checksum) ? EXIT_SUCCESS : EXIT_FAILURE;
}
