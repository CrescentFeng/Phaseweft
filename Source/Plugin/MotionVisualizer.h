#pragma once

#include <JuceHeader.h>
#include "TempoSync.h"

#include <functional>

class MotionVisualizer final : public juce::Component,
                               private juce::Timer {
public:
    MotionVisualizer(juce::AudioProcessorValueTreeState&, std::function<float()> effectiveRate);
    ~MotionVisualizer() override;

    void paint(juce::Graphics&) override;

private:
    void timerCallback() override;
    [[nodiscard]] float value(const char* parameterId) const noexcept;

    juce::AudioProcessorValueTreeState& state_;
    std::function<float()> effectiveRate_;
    double phase_{};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MotionVisualizer)
};
