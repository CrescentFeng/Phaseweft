#pragma once

#include "CubicDelayLine.h"
#include "SignalAnalysis.h"
#include "SmoothedValue.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <numbers>

namespace motionfield {

enum class Mode : int {
    classic = 0,
    throughZero,
    transient,
    field,
    count
};

struct Parameters {
    Mode mode{Mode::field};
    double rateHz{0.22};
    double motion{0.65};
    double focus{0.0};
    double depth{0.78};
    double spread{0.55};
    double character{0.20};
    double feedback{0.38};
    double mix{0.55};
};

struct StereoFrame {
    double left{};
    double right{};
};

class StereoDelayVoice {
public:
    void prepare(std::size_t maximumDelaySamples) {
        left_.prepare(maximumDelaySamples);
        right_.prepare(maximumDelaySamples);
        reset();
    }

    void reset() noexcept {
        left_.reset();
        right_.reset();
    }

    [[nodiscard]] StereoFrame process(double inputLeft, double inputRight,
                                      double delayLeft, double delayRight,
                                      double feedback, double crossFeedback,
                                      double character) noexcept {
        const auto wetLeft = static_cast<double>(left_.read(delayLeft));
        const auto wetRight = static_cast<double>(right_.read(delayRight));
        const auto cross = std::clamp(crossFeedback, 0.0, 0.92);
        const auto feedbackLeft = (1.0 - cross) * wetLeft + cross * wetRight;
        const auto feedbackRight = (1.0 - cross) * wetRight + cross * wetLeft;
        left_.push(inputLeft + feedback * colour(feedbackLeft, character));
        right_.push(inputRight + feedback * colour(feedbackRight, character));
        return {wetLeft, wetRight};
    }

private:
    static double colour(double input, double amount) noexcept {
        const auto drive = 1.0 + 5.0 * std::clamp(amount, 0.0, 1.0);
        // Unity slope around zero keeps the feedback-loop small-signal gain
        // controlled exclusively by the feedback parameter.
        const auto saturated = std::tanh(drive * input) / drive;
        return input + std::clamp(amount, 0.0, 1.0) * (saturated - input);
    }

    CubicDelayLine<double> left_{};
    CubicDelayLine<double> right_{};
};

class MotionFieldEngine {
public:
    void prepare(double sampleRate, double maximumDelayMs = 12.0) {
        sampleRate_ = std::max(1.0, sampleRate);
        centreDelaySamples_ = std::round(sampleRate_ * 0.004);
        excursionSamples_ = std::max(1.0, centreDelaySamples_ - sampleRate_ * 0.0005);
        const auto maximumSamples = static_cast<std::size_t>(
            std::ceil(sampleRate_ * maximumDelayMs * 0.001));

        dryLeft_.prepare(maximumSamples);
        dryRight_.prepare(maximumSamples);
        classicVoice_.prepare(maximumSamples);
        throughZeroVoice_.prepare(maximumSamples);
        transientVoice_.prepare(maximumSamples);
        sustainVoice_.prepare(maximumSamples);
        for (auto& voice : fieldVoices_) voice.prepare(maximumSamples);
        leftSplitter_.prepare(sampleRate_);
        rightSplitter_.prepare(sampleRate_);
        analyzer_.prepare(sampleRate_);

        for (auto* smoother : parameterSmoothers()) smoother->prepare(sampleRate_);
        for (auto& smoother : modeWeights_) smoother.prepare(sampleRate_, 0.04);
        setParameters(parameters_, true);
        reset();
    }

    void reset() noexcept {
        dryLeft_.reset();
        dryRight_.reset();
        classicVoice_.reset();
        throughZeroVoice_.reset();
        transientVoice_.reset();
        sustainVoice_.reset();
        for (auto& voice : fieldVoices_) voice.reset();
        leftSplitter_.reset();
        rightSplitter_.reset();
        analyzer_.reset();
        mainPhase_ = 0.0;
        fieldPhases_ = {0.0, 0.7, 1.4};
    }

    void setParameters(const Parameters& requested, bool immediate = false) noexcept {
        parameters_.mode = sanitiseMode(requested.mode);
        parameters_.rateHz = std::clamp(requested.rateHz, 0.02, 12.0);
        parameters_.motion = std::clamp(requested.motion, 0.0, 1.0);
        parameters_.focus = std::clamp(requested.focus, -1.0, 1.0);
        parameters_.depth = std::clamp(requested.depth, 0.0, 1.0);
        parameters_.spread = std::clamp(requested.spread, 0.0, 1.0);
        parameters_.character = std::clamp(requested.character, 0.0, 1.0);
        parameters_.feedback = std::clamp(requested.feedback, -0.88, 0.88);
        parameters_.mix = std::clamp(requested.mix, 0.0, 1.0);

        const std::array<double, 8> values{parameters_.rateHz, parameters_.motion,
            parameters_.focus, parameters_.depth, parameters_.spread,
            parameters_.character, parameters_.feedback, parameters_.mix};
        auto smoothers = parameterSmoothers();
        for (std::size_t i = 0; i < smoothers.size(); ++i) {
            if (immediate) smoothers[i]->reset(values[i]);
            else smoothers[i]->setTarget(values[i]);
        }

        const auto modeIndex = static_cast<std::size_t>(parameters_.mode);
        for (std::size_t i = 0; i < modeWeights_.size(); ++i) {
            const auto target = i == modeIndex ? 1.0 : 0.0;
            if (immediate) modeWeights_[i].reset(target);
            else modeWeights_[i].setTarget(target);
        }
    }

    [[nodiscard]] StereoFrame process(double inputLeft, double inputRight) noexcept {
        const auto rate = rate_.next();
        const auto motion = motion_.next();
        const auto focus = focus_.next();
        const auto depth = depth_.next();
        const auto spread = spread_.next();
        const auto character = character_.next();
        const auto feedback = feedback_.next();
        const auto mix = mix_.next();

        const auto dry = StereoFrame{static_cast<double>(dryLeft_.read(centreDelaySamples_)),
                                     static_cast<double>(dryRight_.read(centreDelaySamples_))};
        dryLeft_.push(inputLeft);
        dryRight_.push(inputRight);

        const auto leftBands = leftSplitter_.process(inputLeft);
        const auto rightBands = rightSplitter_.process(inputRight);
        std::array<double, 3> monoBands{};
        for (std::size_t i = 0; i < monoBands.size(); ++i)
            monoBands[i] = 0.5 * (leftBands[i] + rightBands[i]);
        const auto analysis = analyzer_.process(0.5 * (inputLeft + inputRight), monoBands);

        const auto classic = processClassic(inputLeft, inputRight, rate, depth,
                                            spread, feedback, character);
        const auto throughZero = processThroughZero(inputLeft, inputRight, depth,
                                                    spread, feedback, character);
        const auto transient = processTransient(inputLeft, inputRight, analysis,
                                                depth, spread, feedback, character);
        const auto field = processField(leftBands, rightBands, analysis, motion, focus,
                                        depth, spread, feedback, character);

        const std::array<StereoFrame, 4> modeOutputs{classic, throughZero, transient, field};
        StereoFrame wet{};
        for (std::size_t i = 0; i < modeOutputs.size(); ++i) {
            const auto weight = modeWeights_[i].next();
            wet.left += weight * modeOutputs[i].left;
            wet.right += weight * modeOutputs[i].right;
        }

        advancePhases(rate, motion);
        return {safetyLimit((1.0 - mix) * dry.left + mix * wet.left),
                safetyLimit((1.0 - mix) * dry.right + mix * wet.right)};
    }

    [[nodiscard]] int latencySamples() const noexcept {
        return static_cast<int>(std::lround(centreDelaySamples_));
    }

    void resetModulationPhase() noexcept {
        mainPhase_ = 0.0;
        fieldPhases_ = {0.0, 0.7, 1.4};
    }

private:
    using SmootherPointers = std::array<SmoothedValue*, 8>;

    [[nodiscard]] SmootherPointers parameterSmoothers() noexcept {
        return {&rate_, &motion_, &focus_, &depth_, &spread_,
                &character_, &feedback_, &mix_};
    }

    static Mode sanitiseMode(Mode mode) noexcept {
        const auto value = std::clamp(static_cast<int>(mode), 0,
                                      static_cast<int>(Mode::count) - 1);
        return static_cast<Mode>(value);
    }

    [[nodiscard]] StereoFrame processClassic(double left, double right, double,
                                             double depth, double spread,
                                             double feedback, double character) noexcept {
        const auto unipolarLeft = 0.5 * (std::sin(mainPhase_) + 1.0);
        const auto unipolarRight = 0.5 * (std::sin(mainPhase_ + spread * 0.65) + 1.0);
        const auto fixedOffset = sampleRate_ * 0.00035;
        const auto range = sampleRate_ * 0.0032 * depth;
        return classicVoice_.process(left, right,
            centreDelaySamples_ + fixedOffset + range * unipolarLeft,
            centreDelaySamples_ + fixedOffset + range * unipolarRight,
            feedback, 0.35 * spread, character);
    }

    [[nodiscard]] StereoFrame processThroughZero(double left, double right,
                                                 double depth, double spread,
                                                 double feedback, double character) noexcept {
        const auto leftMod = std::sin(mainPhase_);
        const auto rightMod = std::sin(mainPhase_ + spread * 0.9);
        return throughZeroVoice_.process(left, right,
            centreDelaySamples_ + excursionSamples_ * depth * leftMod,
            centreDelaySamples_ + excursionSamples_ * depth * rightMod,
            feedback, 0.55 * spread, character);
    }

    [[nodiscard]] StereoFrame processTransient(double left, double right,
                                               const AnalysisFrame& analysis,
                                               double depth, double spread,
                                               double feedback, double character) noexcept {
        const auto transientGain = analysis.transientAmount;
        const auto sustainGain = 1.0 - transientGain;
        const auto transientPhase = 1.83 * mainPhase_ + 0.6 * analysis.spectralPosition;
        const auto sustainPhase = 0.63 * mainPhase_ - 0.35 * analysis.spectralPosition;
        const auto transientWet = transientVoice_.process(left * transientGain,
            right * transientGain,
            centreDelaySamples_ + excursionSamples_ * depth * 0.46 * std::sin(transientPhase),
            centreDelaySamples_ + excursionSamples_ * depth * 0.46
                * std::sin(transientPhase + spread * 1.25),
            feedback * 0.35, 0.25 * spread, character);
        const auto sustainWet = sustainVoice_.process(left * sustainGain, right * sustainGain,
            centreDelaySamples_ + excursionSamples_ * depth * std::sin(sustainPhase),
            centreDelaySamples_ + excursionSamples_ * depth
                * std::sin(sustainPhase + spread * 1.05),
            feedback, 0.70 * spread, character);
        return {transientWet.left + sustainWet.left,
                transientWet.right + sustainWet.right};
    }

    [[nodiscard]] StereoFrame processField(const std::array<double, 3>& leftBands,
                                           const std::array<double, 3>& rightBands,
                                           const AnalysisFrame& analysis,
                                           double motion, double focus, double depth,
                                           double spread, double feedback,
                                           double character) noexcept {
        constexpr std::array<double, 3> positions{-1.0, 0.0, 1.0};
        StereoFrame output{};
        for (std::size_t i = 0; i < fieldVoices_.size(); ++i) {
            const auto focusDistance = 0.5 * std::abs(positions[i] - focus);
            const auto focusWeight = 0.25 + 0.75 * (1.0 - std::clamp(focusDistance, 0.0, 1.0));
            const auto adaptive = motion * (0.78 * analysis.spectralPosition * positions[i]
                                           + 0.52 * analysis.transientAmount);
            const auto leftPhase = fieldPhases_[i] + adaptive;
            const auto rightPhase = fieldPhases_[i] + adaptive
                                  + spread * (0.55 + 0.28 * static_cast<double>(i));
            const auto bandDepth = depth * focusWeight;
            const auto bandWet = fieldVoices_[i].process(leftBands[i], rightBands[i],
                centreDelaySamples_ + excursionSamples_ * bandDepth * std::sin(leftPhase),
                centreDelaySamples_ + excursionSamples_ * bandDepth * std::sin(rightPhase),
                feedback * (0.72 + 0.12 * static_cast<double>(i)),
                spread * (0.35 + 0.20 * static_cast<double>(i)), character);
            output.left += bandWet.left;
            output.right += bandWet.right;
        }
        return output;
    }

    void advancePhases(double rate, double motion) noexcept {
        const auto mainIncrement = 2.0 * std::numbers::pi * rate / sampleRate_;
        mainPhase_ = wrapPhase(mainPhase_ + mainIncrement);
        constexpr std::array<double, 3> ratios{0.73, 1.0, 1.37};
        for (std::size_t i = 0; i < fieldPhases_.size(); ++i) {
            const auto activity = 0.75 + motion * (0.25 + 0.12 * static_cast<double>(i));
            fieldPhases_[i] = wrapPhase(fieldPhases_[i] + mainIncrement * ratios[i] * activity);
        }
    }

    static double wrapPhase(double phase) noexcept {
        constexpr auto twoPi = 2.0 * std::numbers::pi;
        if (phase >= twoPi) phase -= twoPi;
        return phase;
    }

    static double safetyLimit(double input) noexcept {
        if (!std::isfinite(input)) return 0.0;
        if (std::abs(input) <= 4.0) return input;
        return 4.0 * std::tanh(input / 4.0);
    }

    Parameters parameters_{};
    double sampleRate_{48000.0};
    double centreDelaySamples_{192.0};
    double excursionSamples_{168.0};
    double mainPhase_{};
    std::array<double, 3> fieldPhases_{0.0, 0.7, 1.4};

    CubicDelayLine<double> dryLeft_{};
    CubicDelayLine<double> dryRight_{};
    StereoDelayVoice classicVoice_{};
    StereoDelayVoice throughZeroVoice_{};
    StereoDelayVoice transientVoice_{};
    StereoDelayVoice sustainVoice_{};
    std::array<StereoDelayVoice, 3> fieldVoices_{};
    ThreeBandSplitter leftSplitter_{};
    ThreeBandSplitter rightSplitter_{};
    ContentAnalyzer analyzer_{};

    SmoothedValue rate_{}, motion_{}, focus_{}, depth_{}, spread_{};
    SmoothedValue character_{}, feedback_{}, mix_{};
    std::array<SmoothedValue, 4> modeWeights_{};
};

} // namespace motionfield
