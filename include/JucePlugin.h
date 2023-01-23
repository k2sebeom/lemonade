#include "JuceHeader.h"
#include "Plugin.h"
#include "JuceUtils.h"



namespace Ade {
template <typename ProcessorType> class JucePlugin: public Plugin {
public:
    virtual ~JucePlugin(){};

    void process(float *data) {
        float floatData[16];
        for(int i = 0; i < 16; i++) {
            floatData[i] = sin(i);
        }

        for(int i = 0; i < 16; i++) {
            std::cout << floatData[i] << std::endl;
        }

        juce::AudioBuffer buffer = arrayToJuceBuffer<float>(floatData, 2, 8);
        std::cout << buffer.getNumSamples() << std::endl;
        std::cout << "Processing with Juce" << std::endl;

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = 44100;
        spec.maximumBlockSize = static_cast<juce::uint32>(buffer.getNumSamples());
        spec.numChannels = static_cast<juce::uint32>(buffer.getNumChannels());

        processor.prepare(spec);
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);

        processor.process(context);

        float *result = juceBufferToArray<float>(buffer);

        for(int i = 0; i < 16; i++) {
            std::cout << result[i] << std::endl;
        }

        delete[] result;
    };
    
private:
    ProcessorType processor;
};
}