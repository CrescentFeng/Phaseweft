#pragma once

#include <JuceHeader.h>
#include "FactoryPresets.h"
#include "MotionFieldEngine.h"
#include "PluginStateMigration.h"
#include "TempoSync.h"

#include <array>
#include <atomic>

class MotionFieldProcessor final : public juce::AudioProcessor {
public:
    MotionFieldProcessor();
    ~MotionFieldProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 1.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::AudioProcessorValueTreeState& state() noexcept { return parameters_; }
    const juce::AudioProcessorValueTreeState& state() const noexcept { return parameters_; }
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
    void applyFactoryPreset(std::size_t index);
    [[nodiscard]] int currentFactoryPresetIndex() const noexcept;
    void undo();
    void redo();
    [[nodiscard]] bool canUndo() const;
    [[nodiscard]] bool canRedo() const;
    void selectAB(int slot);
    [[nodiscard]] int currentAB() const noexcept { return currentAB_.load(); }
    [[nodiscard]] float effectiveRateHz() const noexcept { return effectiveRateHz_.load(); }

private:
    motionfield::Parameters readParameters() const noexcept;

    juce::UndoManager undoManager_{100};
    juce::AudioProcessorValueTreeState parameters_;
    juce::ValueTree canonicalDefaultState_;
    motionfield::MotionFieldEngine engine_{};
    bool wasPlaying_{};
    std::array<juce::ValueTree, 2> abStates_{};
    std::atomic<int> currentAB_{0};
    std::atomic<float> effectiveRateHz_{0.22f};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MotionFieldProcessor)
};
