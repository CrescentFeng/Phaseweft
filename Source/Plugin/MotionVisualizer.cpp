#include "MotionVisualizer.h"

#include <array>
#include <cmath>
#include <numbers>

MotionVisualizer::MotionVisualizer(juce::AudioProcessorValueTreeState& state,
                                   std::function<float()> effectiveRate)
    : state_(state), effectiveRate_(std::move(effectiveRate)) {
    setTitle("Phaseweft trajectory display");
    setDescription("Animated low, mid and high modulation trajectories derived from the current controls");
    setAccessible(true);
    startTimerHz(30);
}

MotionVisualizer::~MotionVisualizer() {
    stopTimer();
}

float MotionVisualizer::value(const char* parameterId) const noexcept {
    if (const auto* parameter = state_.getRawParameterValue(parameterId))
        return parameter->load();
    return 0.0f;
}

void MotionVisualizer::timerCallback() {
    const auto rate = static_cast<double>(effectiveRate_());
    phase_ = std::fmod(phase_ + 2.0 * std::numbers::pi * rate / 30.0,
                      2.0 * std::numbers::pi);
    repaint();
}

void MotionVisualizer::paint(juce::Graphics& graphics) {
    auto bounds = getLocalBounds().toFloat().reduced(1.0f);
    graphics.setColour(juce::Colour(0xcc091116));
    graphics.fillRoundedRectangle(bounds, 10.0f);
    graphics.setColour(juce::Colour(0xff29434b));
    graphics.drawRoundedRectangle(bounds, 10.0f, 1.0f);

    auto graph = bounds.reduced(18.0f, 24.0f);
    graphics.setColour(juce::Colour(0x223f5a63));
    for (int i = 1; i < 6; ++i) {
        const auto x = graph.getX() + graph.getWidth() * static_cast<float>(i) / 6.0f;
        graphics.drawVerticalLine(static_cast<int>(x), graph.getY(), graph.getBottom());
    }

    const auto depth = value("depth");
    const auto motion = value("motion");
    const auto focus = value("focus");
    const auto spread = value("spread");
    constexpr std::array<float, 3> positions{-1.0f, 0.0f, 1.0f};
    const std::array<juce::Colour, 3> colours{
        juce::Colour(0xff65a8ff), juce::Colour(0xff72ead2), juce::Colour(0xffffc765)};
    constexpr std::array<const char*, 3> labels{"LOW", "MID", "HIGH"};

    for (std::size_t band = 0; band < positions.size(); ++band) {
        const auto rowHeight = graph.getHeight() / 3.0f;
        const auto centreY = graph.getY() + rowHeight * (static_cast<float>(band) + 0.5f);
        const auto focusWeight = 0.25f + 0.75f
            * (1.0f - juce::jlimit(0.0f, 1.0f, 0.5f * std::abs(positions[band] - focus)));
        const auto amplitude = rowHeight * 0.34f * depth * focusWeight;
        const auto phaseOffset = static_cast<double>(band) * (0.72 + 0.95 * motion)
                               + static_cast<double>(spread) * 0.4;
        juce::Path path;
        constexpr int points = 150;
        for (int point = 0; point < points; ++point) {
            const auto normalised = static_cast<float>(point) / static_cast<float>(points - 1);
            const auto x = graph.getX() + normalised * graph.getWidth();
            const auto harmonic = 1.0 + static_cast<double>(band) * 0.22;
            const auto y = centreY + amplitude * static_cast<float>(
                std::sin(phase_ * harmonic + phaseOffset
                         + normalised * static_cast<float>(2.0 * std::numbers::pi)
                           * (1.0f + 0.6f * motion)));
            if (point == 0) path.startNewSubPath(x, y);
            else path.lineTo(x, y);
        }
        graphics.setColour(colours[band].withAlpha(0.35f + 0.60f * focusWeight));
        graphics.strokePath(path, juce::PathStrokeType(1.5f + focusWeight));
        graphics.setFont(juce::FontOptions(10.0f, juce::Font::bold));
        graphics.drawText(labels[band], static_cast<int>(graph.getX()),
                          static_cast<int>(centreY - 8.0f), 42, 16,
                          juce::Justification::centredLeft);
    }

    const auto mode = juce::roundToInt(value("mode"));
    constexpr std::array<const char*, 4> modeNames{
        "CLASSIC", "THROUGH-ZERO", "TRANSIENT SPLIT", "FIELD MATRIX"};
    graphics.setColour(juce::Colour(0xff8fa8af));
    graphics.setFont(juce::FontOptions(11.0f, juce::Font::bold));
    graphics.drawText(juce::String("CONTROL FIELD / ")
                          + modeNames[static_cast<std::size_t>(juce::jlimit(0, 3, mode))],
                      bounds.toNearestInt().reduced(12, 6),
                      juce::Justification::topRight);
}
