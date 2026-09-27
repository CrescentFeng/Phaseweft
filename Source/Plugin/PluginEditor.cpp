#include "PluginEditor.h"

MotionFieldEditor::MotionFieldEditor(MotionFieldProcessor& owner)
    : AudioProcessorEditor(&owner), processor_(owner),
      visualizer_(owner.state(), [&owner] { return owner.effectiveRateHz(); }) {
    setLookAndFeel(&lookAndFeel_);

    presetSelector_.addItem("FACTORY PRESETS...", 1);
    juce::String previousCategory;
    for (std::size_t i = 0; i < motionfield::factoryPresets.size(); ++i) {
        const auto& preset = motionfield::factoryPresets[i];
        if (previousCategory != preset.category) {
            presetSelector_.addSectionHeading(preset.category);
            previousCategory = preset.category;
        }
        presetSelector_.addItem(preset.name, static_cast<int>(i) + 2);
    }
    presetSelector_.setSelectedId(1, juce::dontSendNotification);
    presetSelector_.setTitle("Factory preset browser");
    presetSelector_.setDescription("Choose a categorized Phaseweft factory preset");
    presetSelector_.onChange = [this] { selectPreset(presetSelector_.getSelectedId() - 2); };

    previousPresetButton_.setTooltip("Previous factory preset");
    nextPresetButton_.setTooltip("Next factory preset");
    previousPresetButton_.onClick = [this] {
        auto index = selectedPresetIndex_ < 0 ? static_cast<int>(motionfield::factoryPresets.size()) - 1
                                             : selectedPresetIndex_ - 1;
        if (index < 0) index = static_cast<int>(motionfield::factoryPresets.size()) - 1;
        presetSelector_.setSelectedId(index + 2, juce::sendNotificationSync);
    };
    nextPresetButton_.onClick = [this] {
        auto index = selectedPresetIndex_ + 1;
        if (index >= static_cast<int>(motionfield::factoryPresets.size())) index = 0;
        presetSelector_.setSelectedId(index + 2, juce::sendNotificationSync);
    };

    modeSelector_.addItemList({"Classic", "Through-Zero", "Transient Split", "Field Matrix"}, 1);
    modeSelector_.setTitle("Processing engine");
    modeSelector_.setDescription("Select the flanging and adaptive-motion algorithm");
    modeAttachment_ = std::make_unique<ComboAttachment>(processor_.state(), "mode", modeSelector_);

    juce::StringArray divisionNames;
    for (const auto& division : motionfield::tempoDivisions) divisionNames.add(division.name);
    divisionSelector_.addItemList(divisionNames, 1);
    divisionSelector_.setTitle("Tempo division");
    divisionSelector_.setDescription("Length of one modulation cycle when tempo sync is active");
    transportSelector_.addItemList({"Free Run", "Restart on Play"}, 1);
    transportSelector_.setTitle("Transport behaviour");
    transportSelector_.setDescription("Keep modulation free-running or restart it when playback starts");

    for (auto* selector : {&presetSelector_, &modeSelector_, &divisionSelector_, &transportSelector_}) {
        selector->setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff17232c));
        selector->setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff435a64));
        selector->setColour(juce::ComboBox::textColourId, juce::Colour(0xffeef5f4));
        addAndMakeVisible(*selector);
    }
    divisionAttachment_ = std::make_unique<ComboAttachment>(
        processor_.state(), "division", divisionSelector_);
    transportAttachment_ = std::make_unique<ComboAttachment>(
        processor_.state(), "transport", transportSelector_);

    syncButton_.setTitle("Tempo sync");
    syncButton_.setDescription("Use host tempo instead of the manual Rate control");
    syncButton_.setColour(juce::ToggleButton::textColourId, juce::Colour(0xffeef5f4));
    syncButton_.setColour(juce::ToggleButton::tickColourId, juce::Colour(0xffffc765));
    addAndMakeVisible(syncButton_);
    syncAttachment_ = std::make_unique<ButtonAttachment>(processor_.state(), "sync", syncButton_);

    modeLabel_.setText("ENGINE", juce::dontSendNotification);
    modeLabel_.setJustificationType(juce::Justification::centredLeft);
    modeLabel_.setColour(juce::Label::textColourId, juce::Colour(0xff8aa0a8));
    addAndMakeVisible(modeLabel_);
    presetInfo_.setText("Select a preset or start from the current controls", juce::dontSendNotification);
    presetInfo_.setJustificationType(juce::Justification::centredLeft);
    presetInfo_.setColour(juce::Label::textColourId, juce::Colour(0xff738b92));
    addAndMakeVisible(presetInfo_);

    constexpr std::array<const char*, 8> ids{
        "rate", "motion", "focus", "depth", "spread", "character", "feedback", "mix"};
    constexpr std::array<const char*, 8> names{
        "RATE", "MOTION", "FOCUS", "DEPTH", "SPREAD", "CHARACTER", "FEEDBACK", "MIX"};
    constexpr std::array<const char*, 8> descriptions{
        "Manual modulation cycles per second", "Amount of content-dependent movement",
        "Move field emphasis between low and high frequencies", "Delay excursion",
        "Stereo phase offset and cross-feedback", "Nonlinear feedback colour",
        "Bipolar feedback amount", "Latency-aligned dry and wet balance"};
    for (std::size_t i = 0; i < knobs_.size(); ++i) {
        auto& knob = knobs_[i];
        knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 82, 21);
        knob.setTitle(names[i]);
        knob.setDescription(descriptions[i]);
        knob.setTooltip(descriptions[i]);
        knob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff72ead2));
        knob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff263b44));
        knob.setColour(juce::Slider::thumbColourId, juce::Colour(0xffffc765));
        knob.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xffe4efed));
        knob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        knob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        addAndMakeVisible(knob);

        labels_[i].setText(names[i], juce::dontSendNotification);
        labels_[i].setJustificationType(juce::Justification::centred);
        labels_[i].setColour(juce::Label::textColourId, juce::Colour(0xffa9bdc1));
        addAndMakeVisible(labels_[i]);
        attachments_[i] = std::make_unique<SliderAttachment>(processor_.state(), ids[i], knob);
    }

    for (auto* button : {&previousPresetButton_, &nextPresetButton_, &undoButton_,
                         &redoButton_, &aButton_, &bButton_})
        addAndMakeVisible(*button);
    undoButton_.setTooltip("Undo the most recent parameter edit or preset load");
    redoButton_.setTooltip("Redo the most recently undone edit");
    aButton_.setTooltip("Recall comparison state A; the current state is saved first");
    bButton_.setTooltip("Recall comparison state B; the current state is saved first");
    undoButton_.onClick = [this] { processor_.undo(); presetSelector_.setSelectedId(1); };
    redoButton_.onClick = [this] { processor_.redo(); presetSelector_.setSelectedId(1); };
    aButton_.onClick = [this] { processor_.selectAB(0); presetSelector_.setSelectedId(1); };
    bButton_.onClick = [this] { processor_.selectAB(1); presetSelector_.setSelectedId(1); };

    addAndMakeVisible(visualizer_);
    startTimerHz(12);
    setResizable(true, true);
    setResizeLimits(820, 520, 1400, 880);
    if (auto* constrainer = getConstrainer()) constrainer->setFixedAspectRatio(1.6);
    setSize(1040, 650);
}

MotionFieldEditor::~MotionFieldEditor() {
    stopTimer();
    setLookAndFeel(nullptr);
}

void MotionFieldEditor::selectPreset(int presetIndex) {
    if (presetIndex < 0 || presetIndex >= static_cast<int>(motionfield::factoryPresets.size())) {
        selectedPresetIndex_ = -1;
        presetInfo_.setText("Custom state", juce::dontSendNotification);
        return;
    }
    selectedPresetIndex_ = presetIndex;
    processor_.applyFactoryPreset(static_cast<std::size_t>(presetIndex));
    const auto& preset = motionfield::factoryPresets[static_cast<std::size_t>(presetIndex)];
    presetInfo_.setText(juce::String(preset.category) + " / " + preset.name,
                        juce::dontSendNotification);
}

void MotionFieldEditor::timerCallback() {
    const auto synced = syncButton_.getToggleState();
    divisionSelector_.setEnabled(synced);
    knobs_[0].setEnabled(!synced);
    undoButton_.setEnabled(processor_.canUndo());
    redoButton_.setEnabled(processor_.canRedo());
    const auto slot = processor_.currentAB();
    aButton_.setToggleState(slot == 0, juce::dontSendNotification);
    bButton_.setToggleState(slot == 1, juce::dontSendNotification);
}

void MotionFieldEditor::paint(juce::Graphics& graphics) {
    juce::ColourGradient background(juce::Colour(0xff091016), 0.0f, 0.0f,
                                    juce::Colour(0xff17313a),
                                    static_cast<float>(getWidth()),
                                    static_cast<float>(getHeight()), false);
    graphics.setGradientFill(background);
    graphics.fillAll();

    graphics.setColour(juce::Colour(0xff72ead2));
    graphics.setFont(juce::FontOptions(30.0f, juce::Font::bold));
    graphics.drawText("PHASEWEFT", 28, 15, getWidth() - 56, 38,
                      juce::Justification::centredLeft);
    graphics.setColour(juce::Colour(0xffffc765));
    graphics.setFont(juce::FontOptions(11.5f, juce::Font::bold));
    graphics.drawText("ADAPTIVE FLANGING INSTRUMENT / PRIVATE BETA 0.6",
                      30, 51, getWidth() - 60, 19, juce::Justification::centredLeft);

    graphics.setColour(juce::Colour(0x3a4e6871));
    graphics.drawRoundedRectangle(getLocalBounds().toFloat().reduced(11.0f), 12.0f, 1.0f);
}

void MotionFieldEditor::resized() {
    auto area = getLocalBounds().reduced(24);
    area.removeFromTop(60);

    auto presetRow = area.removeFromTop(38);
    previousPresetButton_.setBounds(presetRow.removeFromLeft(34).reduced(2));
    presetSelector_.setBounds(presetRow.removeFromLeft(270).reduced(2));
    nextPresetButton_.setBounds(presetRow.removeFromLeft(34).reduced(2));
    presetRow.removeFromLeft(10);
    presetInfo_.setBounds(presetRow.removeFromLeft(230));
    presetRow.removeFromLeft(8);
    bButton_.setBounds(presetRow.removeFromRight(40).reduced(2));
    aButton_.setBounds(presetRow.removeFromRight(40).reduced(2));
    redoButton_.setBounds(presetRow.removeFromRight(66).reduced(2));
    undoButton_.setBounds(presetRow.removeFromRight(66).reduced(2));

    area.removeFromTop(5);
    auto engineRow = area.removeFromTop(38);
    modeLabel_.setBounds(engineRow.removeFromLeft(68));
    modeSelector_.setBounds(engineRow.removeFromLeft(190).reduced(2));
    engineRow.removeFromLeft(14);
    syncButton_.setBounds(engineRow.removeFromLeft(75));
    divisionSelector_.setBounds(engineRow.removeFromLeft(88).reduced(2));
    engineRow.removeFromLeft(8);
    transportSelector_.setBounds(engineRow.removeFromLeft(160).reduced(2));

    area.removeFromTop(7);
    const auto visualHeight = juce::jlimit(120, 210, area.getHeight() * 34 / 100);
    visualizer_.setBounds(area.removeFromTop(visualHeight));
    area.removeFromTop(7);

    const auto knobWidth = area.getWidth() / 4;
    const auto knobHeight = area.getHeight() / 2;
    for (std::size_t i = 0; i < knobs_.size(); ++i) {
        const auto column = static_cast<int>(i % 4U);
        const auto row = static_cast<int>(i / 4U);
        auto cell = juce::Rectangle<int>(area.getX() + column * knobWidth,
                                         area.getY() + row * knobHeight,
                                         knobWidth, knobHeight).reduced(7, 1);
        labels_[i].setBounds(cell.removeFromTop(19));
        knobs_[i].setBounds(cell);
    }
}
