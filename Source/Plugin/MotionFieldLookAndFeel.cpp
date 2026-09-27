#include "MotionFieldLookAndFeel.h"

#include <cmath>

MotionFieldLookAndFeel::MotionFieldLookAndFeel() {
    setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xff132129));
    setColour(juce::PopupMenu::textColourId, juce::Colour(0xffe6efed));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff275348));
    setColour(juce::TextButton::buttonColourId, juce::Colour(0xff182831));
    setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff275348));
    setColour(juce::TextButton::textColourOffId, juce::Colour(0xff9eb2b6));
    setColour(juce::TextButton::textColourOnId, juce::Colour(0xfff2f8f6));
}

void MotionFieldLookAndFeel::drawRotarySlider(juce::Graphics& graphics,
                                              int x, int y, int width, int height,
                                              float sliderPosition,
                                              float rotaryStartAngle,
                                              float rotaryEndAngle,
                                              juce::Slider& slider) {
    auto bounds = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y),
                                         static_cast<float>(width), static_cast<float>(height))
                      .reduced(11.0f);
    const auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const auto centre = bounds.getCentre();
    const auto angle = rotaryStartAngle
                     + sliderPosition * (rotaryEndAngle - rotaryStartAngle);

    graphics.setColour(juce::Colour(0xff0b1419));
    graphics.fillEllipse(bounds.withSizeKeepingCentre(radius * 2.0f, radius * 2.0f));
    graphics.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
    graphics.drawEllipse(bounds.withSizeKeepingCentre(radius * 2.0f, radius * 2.0f), 2.0f);

    juce::Path backgroundArc;
    backgroundArc.addCentredArc(centre.x, centre.y, radius - 3.0f, radius - 3.0f,
                                0.0f, rotaryStartAngle, rotaryEndAngle, true);
    graphics.setColour(juce::Colour(0xff2b424b));
    graphics.strokePath(backgroundArc,
                        juce::PathStrokeType(4.0f, juce::PathStrokeType::curved,
                                             juce::PathStrokeType::rounded));

    juce::Path valueArc;
    valueArc.addCentredArc(centre.x, centre.y, radius - 3.0f, radius - 3.0f,
                           0.0f, rotaryStartAngle, angle, true);
    graphics.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
    graphics.strokePath(valueArc,
                        juce::PathStrokeType(4.0f, juce::PathStrokeType::curved,
                                             juce::PathStrokeType::rounded));

    const auto pointerLength = radius * 0.62f;
    const auto pointer = juce::Point<float>(centre.x + std::sin(angle) * pointerLength,
                                            centre.y - std::cos(angle) * pointerLength);
    graphics.setColour(slider.findColour(juce::Slider::thumbColourId));
    graphics.drawLine(centre.x, centre.y, pointer.x, pointer.y, 2.4f);
    graphics.fillEllipse(pointer.x - 3.0f, pointer.y - 3.0f, 6.0f, 6.0f);
}
