#pragma once

#include <JuceHeader.h>
#include "MotionFieldLookAndFeel.h"
#include "MotionVisualizer.h"
#include "PluginProcessor.h"

#include <array>
#include <memory>

class MotionFieldEditor final : public juce::AudioProcessorEditor,
                                private juce::Timer {
public:
    explicit MotionFieldEditor(MotionFieldProcessor&);
    ~MotionFieldEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void selectPreset(int presetIndex);
    void updatePresetDisplay();

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    MotionFieldProcessor& processor_;
    MotionFieldLookAndFeel lookAndFeel_{};
    MotionVisualizer visualizer_;
    juce::ComboBox presetSelector_{};
    juce::ComboBox modeSelector_{};
    juce::ComboBox divisionSelector_{};
    juce::ComboBox transportSelector_{};
    juce::ToggleButton syncButton_{"SYNC"};
    juce::TextButton previousPresetButton_{"<"};
    juce::TextButton nextPresetButton_{">"};
    juce::TextButton undoButton_{"UNDO"};
    juce::TextButton redoButton_{"REDO"};
    juce::TextButton aButton_{"A"};
    juce::TextButton bButton_{"B"};
    juce::Label modeLabel_{};
    juce::Label presetInfo_{};
    std::unique_ptr<ComboAttachment> modeAttachment_{};
    std::unique_ptr<ComboAttachment> divisionAttachment_{};
    std::unique_ptr<ComboAttachment> transportAttachment_{};
    std::unique_ptr<ButtonAttachment> syncAttachment_{};
    std::array<juce::Slider, 8> knobs_{};
    std::array<juce::Label, 8> labels_{};
    std::array<std::unique_ptr<SliderAttachment>, 8> attachments_{};
    // Use a sentinel distinct from "custom" (-1) so the first editor refresh
    // always initialises the preset label, including for restored custom states.
    int selectedPresetIndex_{-2};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MotionFieldEditor)
};
