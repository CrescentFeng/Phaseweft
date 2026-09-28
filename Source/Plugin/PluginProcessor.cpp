#include "PluginProcessor.h"
#include "PluginEditor.h"

MotionFieldProcessor::MotionFieldProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters_(*this, &undoManager_, motionfield::state::stateRootName, createLayout()) {
    canonicalDefaultState_ = parameters_.copyState();
    abStates_[0] = canonicalDefaultState_.createCopy();
    abStates_[1] = abStates_[0].createCopy();
}

juce::AudioProcessorValueTreeState::ParameterLayout MotionFieldProcessor::createLayout() {
    using Float = juce::AudioParameterFloat;
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"mode", 1}, "Mode",
        juce::StringArray{"Classic", "Through-Zero", "Transient Split", "Field Matrix"}, 3));
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{"sync", 1}, "Tempo Sync", false));
    juce::StringArray divisionNames;
    for (const auto& division : motionfield::tempoDivisions) divisionNames.add(division.name);
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"division", 1}, "Division", divisionNames, 9));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{"transport", 1}, "Transport",
        juce::StringArray{"Free Run", "Restart on Play"}, 0));
    layout.add(std::make_unique<Float>(juce::ParameterID{"rate", 1}, "Rate",
        juce::NormalisableRange<float>{0.02f, 5.0f, 0.001f, 0.35f}, 0.22f,
        juce::AudioParameterFloatAttributes{}.withLabel("Hz")));
    layout.add(std::make_unique<Float>(juce::ParameterID{"motion", 1}, "Motion", 0.0f, 1.0f, 0.65f));
    layout.add(std::make_unique<Float>(juce::ParameterID{"focus", 1}, "Focus", -1.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<Float>(juce::ParameterID{"depth", 1}, "Depth", 0.0f, 1.0f, 0.78f));
    layout.add(std::make_unique<Float>(juce::ParameterID{"spread", 1}, "Spread", 0.0f, 1.0f, 0.55f));
    layout.add(std::make_unique<Float>(juce::ParameterID{"character", 1}, "Character", 0.0f, 1.0f, 0.20f));
    layout.add(std::make_unique<Float>(juce::ParameterID{"feedback", 1}, "Feedback", -0.88f, 0.88f, 0.38f));
    layout.add(std::make_unique<Float>(juce::ParameterID{"mix", 1}, "Mix", 0.0f, 1.0f, 0.55f));
    return layout;
}

motionfield::Parameters MotionFieldProcessor::readParameters() const noexcept {
    motionfield::Parameters result;
    result.mode = static_cast<motionfield::Mode>(
        juce::roundToInt(parameters_.getRawParameterValue("mode")->load()));
    result.rateHz = parameters_.getRawParameterValue("rate")->load();
    result.motion = parameters_.getRawParameterValue("motion")->load();
    result.focus = parameters_.getRawParameterValue("focus")->load();
    result.depth = parameters_.getRawParameterValue("depth")->load();
    result.spread = parameters_.getRawParameterValue("spread")->load();
    result.character = parameters_.getRawParameterValue("character")->load();
    result.feedback = parameters_.getRawParameterValue("feedback")->load();
    result.mix = parameters_.getRawParameterValue("mix")->load();
    return result;
}

void MotionFieldProcessor::prepareToPlay(double sampleRate, int) {
    engine_.prepare(sampleRate);
    engine_.setParameters(readParameters(), true);
    setLatencySamples(engine_.latencySamples());
    wasPlaying_ = false;
}

void MotionFieldProcessor::releaseResources() {
    engine_.reset();
}

bool MotionFieldProcessor::isBusesLayoutSupported(const BusesLayout& layout) const {
    const auto output = layout.getMainOutputChannelSet();
    return (output == juce::AudioChannelSet::mono() || output == juce::AudioChannelSet::stereo())
        && output == layout.getMainInputChannelSet();
}

void MotionFieldProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::ScopedNoDenormals noDenormals;
    auto parameters = readParameters();
    auto bpm = 120.0;
    auto isPlaying = false;
    if (auto* playHead = getPlayHead()) {
        if (const auto position = playHead->getPosition()) {
            if (const auto hostBpm = position->getBpm()) bpm = *hostBpm;
            isPlaying = position->getIsPlaying();
        }
    }
    const auto tempoSync = parameters_.getRawParameterValue("sync")->load() >= 0.5f;
    if (tempoSync) {
        const auto division = juce::roundToInt(
            parameters_.getRawParameterValue("division")->load());
        parameters.rateHz = motionfield::tempoRateHz(bpm, division);
    }
    effectiveRateHz_.store(static_cast<float>(parameters.rateHz));
    const auto transportMode = juce::roundToInt(
        parameters_.getRawParameterValue("transport")->load());
    if (transportMode == 1 && isPlaying && !wasPlaying_) engine_.resetModulationPhase();
    wasPlaying_ = isPlaying;
    engine_.setParameters(parameters);
    const auto channels = buffer.getNumChannels();
    auto* left = buffer.getWritePointer(0);
    auto* right = channels > 1 ? buffer.getWritePointer(1) : nullptr;
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        const auto inputLeft = static_cast<double>(left[sample]);
        const auto inputRight = right != nullptr ? static_cast<double>(right[sample]) : inputLeft;
        const auto output = engine_.process(inputLeft, inputRight);
        if (right != nullptr) {
            left[sample] = static_cast<float>(output.left);
            right[sample] = static_cast<float>(output.right);
        } else {
            left[sample] = static_cast<float>(0.5 * (output.left + output.right));
        }
    }
}

void MotionFieldProcessor::applyFactoryPreset(std::size_t index) {
    if (index >= motionfield::factoryPresets.size()) return;
    const auto& preset = motionfield::factoryPresets[index];
    undoManager_.beginNewTransaction(juce::String("Load preset: ") + preset.name);
    const auto setParameter = [this](const char* id, float plainValue) {
        if (auto* parameter = parameters_.getParameter(id)) {
            parameter->beginChangeGesture();
            parameter->setValueNotifyingHost(parameter->convertTo0to1(plainValue));
            parameter->endChangeGesture();
        }
    };
    setParameter("mode", static_cast<float>(preset.parameters.mode));
    setParameter("sync", preset.tempoSync ? 1.0f : 0.0f);
    setParameter("division", static_cast<float>(preset.divisionIndex));
    setParameter("transport", static_cast<float>(preset.transportMode));
    setParameter("rate", static_cast<float>(preset.parameters.rateHz));
    setParameter("motion", static_cast<float>(preset.parameters.motion));
    setParameter("focus", static_cast<float>(preset.parameters.focus));
    setParameter("depth", static_cast<float>(preset.parameters.depth));
    setParameter("spread", static_cast<float>(preset.parameters.spread));
    setParameter("character", static_cast<float>(preset.parameters.character));
    setParameter("feedback", static_cast<float>(preset.parameters.feedback));
    setParameter("mix", static_cast<float>(preset.parameters.mix));
}

int MotionFieldProcessor::currentFactoryPresetIndex() const noexcept {
    const auto equals = [](float actual, double expected) {
        constexpr auto tolerance = 5.0e-4f;
        return std::abs(actual - static_cast<float>(expected)) <= tolerance;
    };
    const auto current = readParameters();
    const auto sync = parameters_.getRawParameterValue("sync")->load() >= 0.5f;
    const auto division = juce::roundToInt(
        parameters_.getRawParameterValue("division")->load());
    const auto transport = juce::roundToInt(
        parameters_.getRawParameterValue("transport")->load());

    for (std::size_t index = 0; index < motionfield::factoryPresets.size(); ++index) {
        const auto& preset = motionfield::factoryPresets[index];
        const auto& expected = preset.parameters;
        if (current.mode == expected.mode
            && sync == preset.tempoSync
            && division == preset.divisionIndex
            && transport == preset.transportMode
            && equals(static_cast<float>(current.rateHz), expected.rateHz)
            && equals(static_cast<float>(current.motion), expected.motion)
            && equals(static_cast<float>(current.focus), expected.focus)
            && equals(static_cast<float>(current.depth), expected.depth)
            && equals(static_cast<float>(current.spread), expected.spread)
            && equals(static_cast<float>(current.character), expected.character)
            && equals(static_cast<float>(current.feedback), expected.feedback)
            && equals(static_cast<float>(current.mix), expected.mix))
            return static_cast<int>(index);
    }
    return -1;
}

void MotionFieldProcessor::undo() {
    undoManager_.undo();
}

void MotionFieldProcessor::redo() {
    undoManager_.redo();
}

bool MotionFieldProcessor::canUndo() const {
    return undoManager_.canUndo();
}

bool MotionFieldProcessor::canRedo() const {
    return undoManager_.canRedo();
}

void MotionFieldProcessor::selectAB(int requestedSlot) {
    const auto target = juce::jlimit(0, 1, requestedSlot);
    const auto current = currentAB_.load();
    if (target == current) return;
    abStates_[static_cast<std::size_t>(current)] = parameters_.copyState();
    if (!abStates_[static_cast<std::size_t>(target)].isValid())
        abStates_[static_cast<std::size_t>(target)] = parameters_.copyState();
    undoManager_.beginNewTransaction(target == 0 ? "Select A" : "Select B");
    parameters_.replaceState(abStates_[static_cast<std::size_t>(target)].createCopy());
    currentAB_.store(target);
}

void MotionFieldProcessor::getStateInformation(juce::MemoryBlock& destination) {
    auto state = parameters_.copyState();
    state.setProperty("schemaVersion", motionfield::state::currentSchemaVersion, nullptr);
    if (auto xml = state.createXml()) copyXmlToBinary(*xml, destination);
}

void MotionFieldProcessor::setStateInformation(const void* data, int size) {
    if (auto xml = getXmlFromBinary(data, size); xml != nullptr) {
        auto restored = juce::ValueTree::fromXml(*xml);
        if (motionfield::state::migrateToCurrent(restored, canonicalDefaultState_)) {
            parameters_.replaceState(restored);
            abStates_[0] = parameters_.copyState();
            abStates_[1] = abStates_[0].createCopy();
            currentAB_.store(0);
            undoManager_.clearUndoHistory();
        }
    }
}

juce::AudioProcessorEditor* MotionFieldProcessor::createEditor() {
    return new MotionFieldEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new MotionFieldProcessor();
}
