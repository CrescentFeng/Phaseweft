set(MOTION_FIELD_FORMATS Standalone VST3)
if(APPLE)
  list(APPEND MOTION_FIELD_FORMATS AU)
endif()

juce_add_plugin(MotionField
  COMPANY_NAME "Crescent Audio"
  BUNDLE_ID "audio.crescent.phaseweft"
  PLUGIN_MANUFACTURER_CODE Crsc
  PLUGIN_CODE Pwf1
  FORMATS ${MOTION_FIELD_FORMATS}
  PRODUCT_NAME "Phaseweft"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  AU_SANDBOX_SAFE TRUE
  HARDENED_RUNTIME_ENABLED TRUE
  VST3_CATEGORIES Fx Modulation Stereo
  COPY_PLUGIN_AFTER_BUILD FALSE)

juce_generate_juce_header(MotionField)

target_sources(MotionField PRIVATE
  PluginProcessor.cpp PluginProcessor.h PluginStateMigration.h
  PluginEditor.cpp PluginEditor.h
  MotionFieldLookAndFeel.cpp MotionFieldLookAndFeel.h
  MotionVisualizer.cpp MotionVisualizer.h)

target_include_directories(MotionField PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../DSP)

target_link_libraries(MotionField PRIVATE
  motion_field_core
  juce::juce_audio_utils
  juce::juce_dsp
  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags)

target_compile_definitions(MotionField PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_VST3_CAN_REPLACE_VST2=0)

add_executable(motion_field_plugin_state_tests
  ${CMAKE_CURRENT_SOURCE_DIR}/../../tests/PluginStateTests.cpp)
target_include_directories(motion_field_plugin_state_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../DSP
  ${CMAKE_CURRENT_BINARY_DIR}/MotionField_artefacts/JuceLibraryCode)
target_link_libraries(motion_field_plugin_state_tests PRIVATE
  MotionField
  juce::juce_audio_utils
  juce::juce_dsp
  juce::juce_recommended_config_flags
  juce::juce_recommended_warning_flags)
add_test(NAME motion_field_plugin_state COMMAND motion_field_plugin_state_tests)

add_executable(motion_field_ui_snapshot EXCLUDE_FROM_ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/../../tests/RenderUISnapshot.cpp)
target_include_directories(motion_field_ui_snapshot PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../DSP
  ${CMAKE_CURRENT_BINARY_DIR}/MotionField_artefacts/JuceLibraryCode)
target_link_libraries(motion_field_ui_snapshot PRIVATE
  MotionField
  juce::juce_audio_utils
  juce::juce_dsp
  juce::juce_recommended_config_flags
  juce::juce_recommended_warning_flags)
