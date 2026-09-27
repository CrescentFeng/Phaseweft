#pragma once

#include <JuceHeader.h>

class MotionFieldLookAndFeel final : public juce::LookAndFeel_V4 {
public:
    MotionFieldLookAndFeel();

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPosition, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override;
};
