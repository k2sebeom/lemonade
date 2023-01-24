#pragma once

#include "JuceHeader.h"
#include "Plugin.h"
#include "JuceUtils.h"
#include <thread>


namespace Ade {

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

        pluginInstance->enableAllBuses();
        pluginInstance->reset();
    }

    ~ExternalPlugin() {
        pluginInstance->reset();
        juce::DeletedAtShutdown::deleteAll();   
        juce::MessageManager::deleteInstance();
    };

    void prepare(double sampleRate, unsigned int nChannels, unsigned int nFrames) {
        // juce::dsp::ProcessSpec spec;
        // spec.sampleRate = sampleRate;
        // spec.maximumBlockSize = static_cast<juce::uint32>(nFrames);
        // spec.numChannels = static_cast<juce::uint32>(nChannels);

        // processor.prepare(spec);
        // lastSpec = spec;
        // isPrepared = true;
    }

    void process(float *data, double sampleRate, unsigned int nChannels, unsigned int nFrames) {
        // if(nFrames == 0) {
        //     return;
        // }

        // if(!isPrepared || lastSpec.numChannels != nChannels || lastSpec.sampleRate != sampleRate || lastSpec.maximumBlockSize < nFrames) {
        //     prepare(sampleRate, nChannels, nFrames);
        // }

        // juce::AudioBuffer buffer = arrayToJuceBuffer<float>(data, nChannels, nFrames);
        
        // auto block = juce::dsp::AudioBlock<float>(buffer);
        // auto context = juce::dsp::ProcessContextReplacing<float>(block);

        // processor.process(context);

        // float *result = juceBufferToArray<float>(buffer);

        // for(int i = 0; i < nChannels * nFrames; i++) {
        //     data[i] = result[i];
        // }

        // delete[] result;
    };

    void showEditor() {
        PluginWindow::openAndWait(*pluginInstance);
    }

    void setEnabled(bool value) {
        // processor.setEnabled(value);
    };
    
protected:


private:
    juce::dsp::ProcessSpec lastSpec;
    bool isPrepared = false;

    juce::String pathToPluginFile;
    juce::PluginDescription foundPluginDescription;
    juce::AudioPluginFormatManager pluginFormatManager;
    juce::KnownPluginList pluginList;
    std::unique_ptr<juce::AudioPluginInstance> pluginInstance;

    // std::unique_ptr<PluginWindow> editorWindow;
};
}