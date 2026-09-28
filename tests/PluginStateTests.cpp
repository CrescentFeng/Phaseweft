#include "../Source/Plugin/PluginProcessor.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string_view>

namespace {

int failures = 0;
constexpr std::array expectedParameterIds{
    "mode", "sync", "division", "transport", "rate", "motion",
    "focus", "depth", "spread", "character", "feedback", "mix"
};

void check(bool condition, std::string_view message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

juce::ValueTree parameterChild(const juce::ValueTree& state, const char* id) {
    return state.getChildWithProperty("id", id);
}

void removeParameter(juce::ValueTree& state, const char* id) {
    if (auto child = parameterChild(state, id); child.isValid())
        state.removeChild(child, nullptr);
}

void setPlainValue(MotionFieldProcessor& processor, const char* id, float value) {
    if (auto* parameter = processor.state().getParameter(id))
        parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
}

float rawValue(const MotionFieldProcessor& processor, const char* id) {
    return processor.state().getRawParameterValue(id)->load();
}

void testPublishedStateIdentityContract() {
    MotionFieldProcessor processor;
    const auto state = processor.state().copyState();

    check(state.hasType(motionfield::state::stateRootName),
          "state root must remain MotionFieldState");
    check(state.getNumChildren() == static_cast<int>(expectedParameterIds.size()),
          "published parameter set must contain exactly the expected identifiers");
    for (const auto* id : expectedParameterIds) {
        check(parameterChild(state, id).isValid(),
              "published parameter identifier must remain present");
    }
}

void testLegacyMigrationUsesCanonicalDefaults() {
    MotionFieldProcessor processor;
    auto legacy = processor.state().copyState();
    removeParameter(legacy, "sync");
    removeParameter(legacy, "division");
    removeParameter(legacy, "transport");
    legacy.setProperty("schemaVersion", 1, nullptr);
    parameterChild(legacy, "feedback").setProperty("value", -0.42, nullptr);

    setPlainValue(processor, "sync", 1.0f);
    setPlainValue(processor, "division", 1.0f);
    setPlainValue(processor, "transport", 1.0f);

    juce::MemoryBlock data;
    juce::AudioProcessor::copyXmlToBinary(*legacy.createXml(), data);
    processor.setStateInformation(data.getData(), static_cast<int>(data.getSize()));

    check(std::abs(rawValue(processor, "feedback") + 0.42f) < 1.0e-5f,
          "legacy value must survive migration");
    check(rawValue(processor, "sync") < 0.5f,
          "missing legacy sync must use canonical default");
    check(std::abs(rawValue(processor, "division") - 9.0f) < 0.5f,
          "missing legacy division must use canonical default");
    check(rawValue(processor, "transport") < 0.5f,
          "missing legacy transport must use canonical default");

    auto migrated = processor.state().copyState();
    check(migrated.hasProperty("schemaVersion")
              && static_cast<int>(migrated.getProperty("schemaVersion"))
                     == motionfield::state::currentSchemaVersion,
          "migrated state must carry the current schema");
}

void testCurrentStateRoundTrip() {
    MotionFieldProcessor source;
    setPlainValue(source, "mode", 2.0f);
    setPlainValue(source, "sync", 1.0f);
    setPlainValue(source, "division", 6.0f);
    setPlainValue(source, "feedback", -0.61f);
    setPlainValue(source, "mix", 0.73f);

    juce::MemoryBlock data;
    source.getStateInformation(data);
    MotionFieldProcessor destination;
    destination.setStateInformation(data.getData(), static_cast<int>(data.getSize()));

    for (const auto* id : {"mode", "sync", "division", "feedback", "mix"})
        check(std::abs(rawValue(source, id) - rawValue(destination, id)) < 1.0e-5f,
              "current state must round-trip exactly");
}

void testFactoryPresetIdentitySurvivesStateRoundTrip() {
    constexpr auto presetIndex = 12U;
    MotionFieldProcessor source;
    source.applyFactoryPreset(presetIndex);
    check(source.currentFactoryPresetIndex() == static_cast<int>(presetIndex),
          "loaded factory preset must be identified from its parameter state");

    juce::MemoryBlock data;
    source.getStateInformation(data);
    MotionFieldProcessor destination;
    destination.setStateInformation(data.getData(), static_cast<int>(data.getSize()));
    check(destination.currentFactoryPresetIndex() == static_cast<int>(presetIndex),
          "factory preset identity must survive host state recall");

    setPlainValue(destination, "mix", 0.17f);
    check(destination.currentFactoryPresetIndex() == -1,
          "edited factory preset must be identified as a custom state");
}

void testMalformedStateIsIgnored() {
    MotionFieldProcessor processor;
    const auto before = rawValue(processor, "feedback");
    const std::array<std::byte, 8> garbage{};
    processor.setStateInformation(garbage.data(), static_cast<int>(garbage.size()));
    check(std::abs(rawValue(processor, "feedback") - before) < 1.0e-7f,
          "malformed binary state must be ignored");

    juce::ValueTree wrongRoot{"WrongProductState"};
    juce::MemoryBlock data;
    juce::AudioProcessor::copyXmlToBinary(*wrongRoot.createXml(), data);
    processor.setStateInformation(data.getData(), static_cast<int>(data.getSize()));
    check(std::abs(rawValue(processor, "feedback") - before) < 1.0e-7f,
          "foreign state root must be ignored");
}

} // namespace

int main() {
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    testPublishedStateIdentityContract();
    testLegacyMigrationUsesCanonicalDefaults();
    testCurrentStateRoundTrip();
    testFactoryPresetIdentitySurvivesStateRoundTrip();
    testMalformedStateIsIgnored();
    if (failures == 0) std::cout << "All plug-in state tests passed\n";
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
