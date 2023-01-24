#pragma once

#include "JuceHeader.h"
#include "Plugin.h"
#include "JuceUtils.h"
#include <vector>
#include <string>


namespace Ade {

typedef struct ExternalPluginInfo {
    std::string name;
    std::string path;
} ExternalPluginInfo;

class PluginWindow: public juce::DocumentWindow {
public:
    PluginWindow(juce::AudioProcessor &processor) : juce::DocumentWindow("Lemonade", juce::LookAndFeel::getDefaultLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId), juce::DocumentWindow::minimiseButton | juce::DocumentWindow::closeButton),
        processor(processor)
    {
        setUsingNativeTitleBar(true);

        if(processor.hasEditor()) {
            auto *editor = processor.createEditorIfNeeded();
            setContentOwned(editor, true);
            setResizable(editor->isResizable(), false);
        }
    }

    ~PluginWindow() override {
        clearContentComponent();
    }

    void closeButtonPressed() override { setVisible(false); }

    static void openAndWait(juce::AudioProcessor &processor) {
        JUCE_AUTORELEASEPOOL {
            PluginWindow window(processor);
            window.show();
            while(window.isVisible()) {
                {
                    juce::MessageManager::getInstance()->runDispatchLoopUntil(10);
                }
            }
        }
        juce::MessageManager::getInstance()->runDispatchLoopUntil(10);
    }

    void show() {
        setVisible(true);
        toFront(true);
        juce::Process::makeForegroundProcess();
    }

private:
    juce::AudioProcessor &processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWindow);
};

class ExternalPlugin: public Plugin {
public:
    static std::vector<ExternalPluginInfo> getInstalledPlugins() {
        juce::MessageManager::getInstance();

        juce::AudioPluginFormatManager pluginFormatManager;
        pluginFormatManager.addDefaultFormats();
        std::vector<ExternalPluginInfo> info;
        for (int i = 0; i < pluginFormatManager.getNumFormats(); ++i)
        {
            juce::AudioPluginFormat *format = pluginFormatManager.getFormat(i);
            for (juce::String pluginIdentifier : format->searchPathsForPlugins(format->getDefaultLocationsToSearch(), true, false)) {
                ExternalPluginInfo p;
                juce::String pluginPath = format->getNameOfPluginFromIdentifier(pluginIdentifier);
                p.path = pluginPath.toStdString();
                p.name = ExternalPlugin::getPluginNamesForFileOfFormat(pluginPath.toStdString(), format).at(0);
                info.push_back(p);
            }
        }
        return info;
    }

    static std::vector<std::string> getPluginNamesForFileOfFormat(std::string filename, juce::AudioPluginFormat *format) {
        juce::MessageManager::getInstance();
        juce::OwnedArray<juce::PluginDescription> typesFound;
 
        format->findAllTypesForFile(typesFound, filename);

        std::vector<std::string> pluginNames;
        for (int i = 0; i < typesFound.size(); i++) {
            pluginNames.push_back(typesFound[i]->name.toStdString());
        }
        return pluginNames;
    }

    ExternalPlugin(std::string pathToPlugin): pathToPluginFile(pathToPlugin) {
        juce::MessageManager::getInstance();
        pluginFormatManager.addDefaultFormats();

        auto pluginFileStripped = pathToPluginFile.trimCharactersAtEnd(juce::File::getSeparatorString());

        // auto fileExists = juce::File::createFileWithoutCheckingPath(pluginFileStripped).exists();

        juce::OwnedArray<juce::PluginDescription> foundDescriptions;

        for (int i = 0; i < pluginFormatManager.getNumFormats(); ++i)
        {
            pluginList.scanAndAddFile(pluginFileStripped, true, foundDescriptions, *pluginFormatManager.getFormat(i));
        }

        foundPluginDescription = *foundDescriptions[0];
        juce::String msg("Fuck");
        pluginInstance = pluginFormatManager.createPluginInstance(foundPluginDescription, 44100, 1024, msg);

        name = pluginInstance->getName().toStdString();
        pluginInstance->enableAllBuses();
        pluginInstance->reset();
    }

    ~ExternalPlugin() {
        pluginInstance->reset();
        juce::DeletedAtShutdown::deleteAll();   
        juce::MessageManager::deleteInstance();
    };

    void prepare(double sampleRate, unsigned int nChannels, unsigned int nFrames) {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = static_cast<juce::uint32>(nFrames);
        spec.numChannels = static_cast<juce::uint32>(nChannels);

        pluginInstance->releaseResources();
        setNumChannels(nChannels);

        pluginInstance->setNonRealtime(true);
        pluginInstance->prepareToPlay(spec.sampleRate, spec.maximumBlockSize);

        lastSpec = spec;
        isPrepared = true;
    }

    void process(float *data, double sampleRate, unsigned int nChannels, unsigned int nFrames) override {
        if(nFrames == 0) {
            return;
        }

        if(!isEnabled) {
            return;
        }

        // If spec is different, prepare the plugin
        if(!isPrepared || lastSpec.numChannels != nChannels || lastSpec.sampleRate != sampleRate || lastSpec.maximumBlockSize < nFrames) {
            prepare(sampleRate, nChannels, nFrames);
        }

        juce::AudioBuffer buffer = arrayToJuceBuffer<float>(data, nChannels, nFrames);
        auto block = juce::dsp::AudioBlock<float>(buffer);

        std::vector<float *> channelPointers(pluginInstance->getTotalNumInputChannels());

        for (size_t i = 0; i < block.getNumChannels(); i++) {
            channelPointers[i] = block.getChannelPointer(i);
        }

        // Initialize possible dummy input channels
        std::vector<std::vector<float>> dummyChannels;
        for (size_t i = block.getNumChannels(); i < channelPointers.size(); i++) {
            std::vector<float> dummyChannel(block.getNumSamples());
            channelPointers[i] = dummyChannel.data();
            dummyChannels.push_back(dummyChannel);
        }

        juce::AudioBuffer<float> audioBuffer(channelPointers.data(),
                                    channelPointers.size(),
                                    block.getNumSamples());
        juce::MidiBuffer emptyMidiBuffer;
        pluginInstance->processBlock(audioBuffer, emptyMidiBuffer);

        // Replace target by the result
        float *result = juceBufferToArray<float>(audioBuffer);

        for(int i = 0; i < nChannels * nFrames; i++) {
            data[i] = result[i];
        }

        delete[] result;
    };

    void showEditor() {
        PluginWindow::openAndWait(*pluginInstance);
    }

    void setEnabled(bool value) override {
        isEnabled = value;
    };

private:
    void setNumChannels(int numChannels) {
        if (numChannels == 0)
            return;
        auto mainInputBus = pluginInstance->getBus(true, 0);
        auto mainOutputBus = pluginInstance->getBus(false, 0);

        // Try to disable all non-main input buses if possible:
        for (int i = 1; i < pluginInstance->getBusCount(true); i++) {
            auto *bus = pluginInstance->getBus(true, i);
            if (bus->isNumberOfChannelsSupported(0))
                bus->enable(false);
        }

        // ...and all non-main output buses too:
        for (int i = 1; i < pluginInstance->getBusCount(false); i++) {
            auto *bus = pluginInstance->getBus(false, i);
            if (bus->isNumberOfChannelsSupported(0))
                bus->enable(false);
        }

        if (mainInputBus->getNumberOfChannels() == numChannels &&
            mainOutputBus->getNumberOfChannels() == numChannels) {
            return;
        }

        // Cache these values in case the plugin fails to update:
        auto previousInputChannelCount = mainInputBus->getNumberOfChannels();
        auto previousOutputChannelCount = mainOutputBus->getNumberOfChannels();

        // Try to change the input and output bus channel counts...
        mainInputBus->setNumberOfChannels(numChannels);
        mainOutputBus->setNumberOfChannels(numChannels);

        // If, post-reload, we still can't use the right number of channels, let's
        // conclude the plugin doesn't allow this channel count.
        if (mainInputBus->getNumberOfChannels() != numChannels ||
            mainOutputBus->getNumberOfChannels() != numChannels) {
            // Reset the bus configuration to what it was before, so we don't
            // leave one of the buses smaller than the other:
            mainInputBus->setNumberOfChannels(previousInputChannelCount);
            mainOutputBus->setNumberOfChannels(previousOutputChannelCount);
        }
    }


    juce::dsp::ProcessSpec lastSpec;
    bool isPrepared = false;

    bool isEnabled = true;

    juce::String pathToPluginFile;
    juce::PluginDescription foundPluginDescription;
    juce::AudioPluginFormatManager pluginFormatManager;
    juce::KnownPluginList pluginList;
    std::unique_ptr<juce::AudioPluginInstance> pluginInstance;
};
}