#include "FactoryPresets.h"
#include "MotionFieldEngine.h"
#include "TempoSync.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numbers>
#include <string>
#include <vector>

namespace {

struct StereoSample {
    float left{};
    float right{};
};

void write16(std::ofstream& output, std::uint16_t value) {
    const std::array<char, 2> bytes{static_cast<char>(value & 0xffU),
                                    static_cast<char>((value >> 8U) & 0xffU)};
    output.write(bytes.data(), 2);
}

void write32(std::ofstream& output, std::uint32_t value) {
    const std::array<char, 4> bytes{static_cast<char>(value & 0xffU),
        static_cast<char>((value >> 8U) & 0xffU),
        static_cast<char>((value >> 16U) & 0xffU),
        static_cast<char>((value >> 24U) & 0xffU)};
    output.write(bytes.data(), 4);
}

void writeWav(const std::filesystem::path& path,
              const std::vector<StereoSample>& samples, int sampleRate) {
    std::ofstream output(path, std::ios::binary);
    const auto dataBytes = static_cast<std::uint32_t>(samples.size() * 4U);
    output.write("RIFF", 4); write32(output, 36U + dataBytes); output.write("WAVE", 4);
    output.write("fmt ", 4); write32(output, 16U); write16(output, 1U); write16(output, 2U);
    write32(output, static_cast<std::uint32_t>(sampleRate));
    write32(output, static_cast<std::uint32_t>(sampleRate * 4));
    write16(output, 4U); write16(output, 16U);
    output.write("data", 4); write32(output, dataBytes);
    for (const auto sample : samples) {
        for (const auto value : {sample.left, sample.right}) {
            const auto clipped = std::clamp(value, -1.0f, 1.0f);
            const auto quantised = static_cast<std::int16_t>(std::lrint(clipped * 32767.0f));
            write16(output, static_cast<std::uint16_t>(quantised));
        }
    }
}

std::vector<StereoSample> makeSource(int sampleRate, double seconds) {
    const auto sampleCount = static_cast<std::size_t>(sampleRate * seconds);
    std::vector<StereoSample> source(sampleCount);
    for (std::size_t i = 0; i < source.size(); ++i) {
        const auto time = static_cast<double>(i) / static_cast<double>(sampleRate);
        const auto beatPhase = std::fmod(time * 2.0, 1.0);
        const auto transientEnvelope = std::exp(-beatPhase * 30.0);
        const auto click = transientEnvelope * std::sin(2.0 * std::numbers::pi
                                                       * (150.0 - 90.0 * beatPhase) * time);
        const auto padEnvelope = std::min(1.0, time * 2.0)
                               * std::min(1.0, (seconds - time) * 2.0);
        const auto padLeft = 0.16 * padEnvelope
            * (std::sin(2.0 * std::numbers::pi * 110.0 * time)
             + 0.45 * std::sin(2.0 * std::numbers::pi * 277.18 * time));
        const auto padRight = 0.16 * padEnvelope
            * (std::sin(2.0 * std::numbers::pi * 110.4 * time)
             + 0.45 * std::sin(2.0 * std::numbers::pi * 329.63 * time));
        source[i] = {static_cast<float>(padLeft + 0.28 * click),
                     static_cast<float>(padRight + 0.28 * click)};
    }
    return source;
}

std::vector<StereoSample> render(const std::vector<StereoSample>& source,
                                 int sampleRate, motionfield::Mode mode) {
    motionfield::MotionFieldEngine engine;
    engine.prepare(sampleRate);
    motionfield::Parameters parameters;
    parameters.mode = mode;
    parameters.rateHz = 0.27;
    parameters.motion = 0.82;
    parameters.focus = 0.15;
    parameters.depth = 0.88;
    parameters.spread = 0.72;
    parameters.character = 0.28;
    parameters.feedback = 0.46;
    parameters.mix = 0.62;
    engine.setParameters(parameters, true);
    std::vector<StereoSample> result(source.size());
    for (std::size_t i = 0; i < source.size(); ++i) {
        const auto output = engine.process(source[i].left, source[i].right);
        result[i] = {static_cast<float>(output.left), static_cast<float>(output.right)};
    }
    return result;
}

std::vector<StereoSample> renderPreset(const std::vector<StereoSample>& source,
                                       int sampleRate,
                                       const motionfield::FactoryPreset& preset) {
    motionfield::MotionFieldEngine engine;
    engine.prepare(sampleRate);
    auto parameters = preset.parameters;
    if (preset.tempoSync)
        parameters.rateHz = motionfield::tempoRateHz(120.0, preset.divisionIndex);
    engine.setParameters(parameters, true);
    std::vector<StereoSample> result(source.size());
    for (std::size_t i = 0; i < source.size(); ++i) {
        const auto output = engine.process(source[i].left, source[i].right);
        result[i] = {static_cast<float>(output.left), static_cast<float>(output.right)};
    }
    return result;
}

} // namespace

int main(int argc, char** argv) {
    const std::filesystem::path outputDirectory = argc > 1 ? argv[1] : "artifacts/audio";
    std::filesystem::create_directories(outputDirectory);
    constexpr int sampleRate = 48000;
    const auto source = makeSource(sampleRate, 8.0);
    writeWav(outputDirectory / "00_dry.wav", source, sampleRate);

    constexpr std::array<motionfield::Mode, 4> modes{motionfield::Mode::classic,
        motionfield::Mode::throughZero, motionfield::Mode::transient, motionfield::Mode::field};
    constexpr std::array<const char*, 4> filenames{"01_classic.wav", "02_through_zero.wav",
                                                   "03_transient_split.wav", "04_motion_field.wav"};
    for (std::size_t i = 0; i < modes.size(); ++i)
        writeWav(outputDirectory / filenames[i], render(source, sampleRate, modes[i]), sampleRate);

    const auto presetDirectory = outputDirectory / "featured-presets";
    std::filesystem::create_directories(presetDirectory);
    constexpr std::array<std::size_t, 6> featured{3U, 4U, 7U, 10U, 12U, 14U};
    constexpr std::array<const char*, 6> presetFiles{"01_transient_halo.wav",
        "02_hi_hat_orbit.wav", "03_zero_jet.wav", "04_pad_drift.wav",
        "05_arp_lattice.wav", "06_negative_spiral.wav"};
    for (std::size_t i = 0; i < featured.size(); ++i) {
        const auto& preset = motionfield::factoryPresets[featured[i]];
        writeWav(presetDirectory / presetFiles[i],
                 renderPreset(source, sampleRate, preset), sampleRate);
    }

    std::cout << "Rendered dry, four engines and six featured presets to "
              << outputDirectory << '\n';
}
