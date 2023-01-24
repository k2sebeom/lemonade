#pragma once

#include "JuceHeader.h"
#include "Plugin.h"
#include "JuceUtils.h"



namespace Ade {
template <typename ProcessorType> class JucePlugin: public Plugin {
public:
    virtual ~JucePlugin(){};

    void prepare(double sampleRate, unsigned int nChannels, unsigned int nFrames) {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = static_cast<juce::uint32>(nFrames);
        spec.numChannels = static_cast<juce::uint32>(nChannels);

        processor.prepare(spec);
        lastSpec = spec;
        isPrepared = true;
    }

    void process(float *data, double sampleRate, unsigned int nChannels, unsigned int nFrames) override {
        if(nFrames == 0) {
            return;
        }

        if(!isPrepared || lastSpec.numChannels != nChannels || lastSpec.sampleRate != sampleRate || lastSpec.maximumBlockSize < nFrames) {
            prepare(sampleRate, nChannels, nFrames);
        }

        juce::AudioBuffer buffer = arrayToJuceBuffer<float>(data, nChannels, nFrames);
        
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);

        processor.process(context);

        float *result = juceBufferToArray<float>(buffer);

        for(int i = 0; i < nChannels * nFrames; i++) {
            data[i] = result[i];
        }

        delete[] result;
    };

    void setEnabled(bool value) override {
        processor.setEnabled(value);
    };
    
protected:
    ProcessorType processor;

private:
    juce::dsp::ProcessSpec lastSpec;
    bool isPrepared = false;
};
}