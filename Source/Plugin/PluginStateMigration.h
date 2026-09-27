#pragma once

#include <JuceHeader.h>

namespace motionfield::state {

// This root predates the Phaseweft product name and is intentionally permanent.
// Changing it would make existing sessions and presets fail identity validation.
inline constexpr auto stateRootName = "MotionFieldState";
inline constexpr int currentSchemaVersion = 2;

inline bool migrateToCurrent(juce::ValueTree& restored,
                             const juce::ValueTree& canonicalDefaults) {
    if (!restored.isValid() || !canonicalDefaults.isValid()
        || !restored.hasType(canonicalDefaults.getType()))
        return false;

    static const juce::Identifier parameterId{"id"};
    for (const auto& defaultChild : canonicalDefaults) {
        if (!defaultChild.hasProperty(parameterId)) continue;
        const auto id = defaultChild.getProperty(parameterId);
        if (!restored.getChildWithProperty(parameterId, id).isValid())
            restored.appendChild(defaultChild.createCopy(), nullptr);
    }

    restored.setProperty("schemaVersion", currentSchemaVersion, nullptr);
    return true;
}

} // namespace motionfield::state
