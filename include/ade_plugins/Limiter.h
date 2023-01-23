#pragma once

#include "JucePlugin.h"
#include <map>


namespace Ade {

class Limiter: public JucePlugin<juce::dsp::Limiter<float>> {
public:
    std::string name = "Limiter";

    Limiter() {
        parameters.push_back("threshold");
        parameters.push_back("release");

        limiterParams.insert({"threshold", -10});
        limiterParams.insert({"release", 100});

        processor.setThreshold(-10);
        processor.setRelease(100);
    }

    PluginParam getParam(std::string paramName) {
        PluginParam p;
        auto i = limiterParams.find(paramName);
        if(i == limiterParams.end()) {
            p.value = -1;
            return p;
        }
        p.value = i->second;

        if(paramName == "threshold") {
            p.min = -60;
            p.max = 0;
            return p;
        }
        if(paramName == "release") {
            p.min = 5;
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
        if(paramName == "release") {
            processor.setRelease(value);
        }
    }

private:
    std::map<std::string, float> limiterParams;
};

}