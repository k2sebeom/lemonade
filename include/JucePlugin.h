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
    }

    void process(float *data, double sampleRate, unsigned int nChannels, unsigned int nFrames) {
        if(nFrames == 0) {
            return;
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
    
protected:
    ProcessorType processor;
};
}