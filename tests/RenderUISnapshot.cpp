#include "PluginProcessor.h"

#include <JuceHeader.h>

#include <iostream>
#include <memory>

int main(int argc, char** argv) {
    const juce::ScopedJuceInitialiser_GUI initialiseJuce;
    const auto output = argc > 1
        ? juce::File::getCurrentWorkingDirectory().getChildFile(argv[1])
        : juce::File::getCurrentWorkingDirectory()
              .getChildFile("artifacts/screenshots/phaseweft-v0.7-editor.png");

    const auto directoryResult = output.getParentDirectory().createDirectory();
    if (directoryResult.failed()) {
        std::cerr << "Could not create screenshot directory: "
                  << directoryResult.getErrorMessage() << '\n';
        return 1;
    }

    MotionFieldProcessor processor;
    processor.prepareToPlay(48000.0, 512);
    processor.applyFactoryPreset(1U); // Utility / Mix Air Motion

    std::unique_ptr<juce::AudioProcessorEditor> editor(processor.createEditor());
    if (editor == nullptr) {
        std::cerr << "Phaseweft did not create an editor\n";
        return 1;
    }

    editor->setSize(1040, 650);
    const auto image = editor->createComponentSnapshot(editor->getLocalBounds(), true, 1.0f);
    if (!image.isValid()) {
        std::cerr << "Phaseweft editor snapshot is invalid\n";
        return 1;
    }

    juce::FileOutputStream stream(output);
    if (!stream.openedOk()) {
        std::cerr << "Could not open screenshot output: "
                  << stream.getStatus().getErrorMessage() << '\n';
        return 1;
    }
    stream.setPosition(0);
    if (!stream.truncate()) {
        std::cerr << "Could not truncate screenshot output\n";
        return 1;
    }

    juce::PNGImageFormat png;
    if (!png.writeImageToStream(image, stream)) {
        std::cerr << "Could not encode Phaseweft editor snapshot\n";
        return 1;
    }
    stream.flush();

    std::cout << "Rendered Phaseweft v0.7 editor snapshot to "
              << output.getFullPathName() << '\n';
    return 0;
}
