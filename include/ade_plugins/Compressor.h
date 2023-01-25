#pragma once

#include "JucePlugin.h"
#include <map>


namespace Ade {

class Compressor: public JucePlugin<juce::dsp::Compressor<float>> {
public:
    std::string name = "Compressor";

    Compressor() {
        parameters.push_back("threshold");
        parameters.push_back("ratio");
        parameters.push_back("attack");
        parameters.push_back("release");

        compressorParams.insert({"threshold", 0});
        compressorParams.insert({"ratio", 1.0});
        compressorParams.insert({"attack", 1.0});
        compressorParams.insert({"release", 100});

        processor.setThreshold(0.0);
        processor.setRatio(1.0);
        processor.setAttack(1.0);
        processor.setRelease(100);
    }

    PluginParam getParam(std::string paramName) {
        PluginParam p;
        auto i = compressorParams.find(paramName);
        if(i == compressorParams.end()) {
            p.value = -1;
            return p;
        }
        p.value = i->second;

        if(paramName == "threshold") {
            p.min = -60;
            p.max = 0;
            return p;
        }
        if(paramName == "ratio") {
            p.min = 1;
            p.max = 24.0;
            return p;
        }
        if(paramName == "attack") {
            p.min = 0;
            p.max = 100;
            return p;
        }
        if(paramName == "release") {
            p.min = 5.0;
            p.max = 1500;
            return p;
        }

        p.value = -1;
        return p;
    }

    void setParam(std::string paramName, float value) {
        if(paramName == "threshold") {
            processor.setThreshold(value);
        }
        if(paramName == "ratio") {
            processor.setRatio(value);
        }
        if(paramName == "attack") {
            processor.setAttack(value);
        }
        if(paramName == "release") {
            processor.setRelease(value);
        }
        compressorParams[paramName] = value;
    }

private:
    std::map<std::string, float> compressorParams;
};

}