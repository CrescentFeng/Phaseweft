#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>

namespace motionfield {

class OnePoleLowpass {
public:
    void prepare(double sampleRate, double cutoffHz) noexcept {
        const auto safeRate = std::max(1.0, sampleRate);
        const auto safeCutoff = std::clamp(cutoffHz, 10.0, 0.45 * safeRate);
        coefficient_ = std::exp(-2.0 * std::numbers::pi * safeCutoff / safeRate);
        state_ = 0.0;
    }

    [[nodiscard]] double process(double input) noexcept {
        state_ = (1.0 - coefficient_) * input + coefficient_ * state_;
        return state_;
    }

    void reset() noexcept { state_ = 0.0; }

private:
    double coefficient_{};
    double state_{};
};

class ThreeBandSplitter {
public:
    void prepare(double sampleRate) noexcept {
        lowCrossover_.prepare(sampleRate, 320.0);
        highCrossover_.prepare(sampleRate, 2800.0);
    }

    void reset() noexcept {
        lowCrossover_.reset();
        highCrossover_.reset();
    }

    [[nodiscard]] std::array<double, 3> process(double input) noexcept {
        const auto low = lowCrossover_.process(input);
        const auto belowHigh = highCrossover_.process(input);
        return {low, belowHigh - low, input - belowHigh};
    }

private:
    OnePoleLowpass lowCrossover_{};
    OnePoleLowpass highCrossover_{};
};

struct AnalysisFrame {
    double transientAmount{};
    double spectralPosition{};
};

class ContentAnalyzer {
public:
    void prepare(double sampleRate) noexcept {
        sampleRate_ = std::max(1.0, sampleRate);
        fastAttack_ = coefficient(0.0015);
        fastRelease_ = coefficient(0.035);
        slowAttack_ = coefficient(0.030);
        slowRelease_ = coefficient(0.220);
        energyCoefficient_ = coefficient(0.040);
        reset();
    }

    void reset() noexcept {
        fastEnvelope_ = slowEnvelope_ = spectralPosition_ = 0.0;
        bandEnergy_.fill(0.0);
    }

    [[nodiscard]] AnalysisFrame process(double monoInput,
                                        const std::array<double, 3>& monoBands) noexcept {
        const auto magnitude = std::abs(monoInput);
        fastEnvelope_ = follow(magnitude, fastEnvelope_, fastAttack_, fastRelease_);
        slowEnvelope_ = follow(magnitude, slowEnvelope_, slowAttack_, slowRelease_);
        const auto transient = std::clamp((fastEnvelope_ - slowEnvelope_)
                                           / (slowEnvelope_ + 0.015), 0.0, 1.0);

        for (std::size_t i = 0; i < bandEnergy_.size(); ++i) {
            const auto instantaneous = monoBands[i] * monoBands[i];
            bandEnergy_[i] = (1.0 - energyCoefficient_) * instantaneous
                           + energyCoefficient_ * bandEnergy_[i];
        }
        const auto total = bandEnergy_[0] + bandEnergy_[1] + bandEnergy_[2] + 1.0e-12;
        const auto targetPosition = (bandEnergy_[2] - bandEnergy_[0]) / total;
        spectralPosition_ += 0.0025 * (targetPosition - spectralPosition_);
        return {transient, std::clamp(spectralPosition_, -1.0, 1.0)};
    }

private:
    [[nodiscard]] double coefficient(double seconds) const noexcept {
        return std::exp(-1.0 / (seconds * sampleRate_));
    }

    static double follow(double input, double previous, double attack, double release) noexcept {
        const auto c = input > previous ? attack : release;
        return (1.0 - c) * input + c * previous;
    }

    double sampleRate_{48000.0};
    double fastAttack_{};
    double fastRelease_{};
    double slowAttack_{};
    double slowRelease_{};
    double energyCoefficient_{};
    double fastEnvelope_{};
    double slowEnvelope_{};
    double spectralPosition_{};
    std::array<double, 3> bandEnergy_{};
};

} // namespace motionfield
