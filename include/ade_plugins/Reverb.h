#pragma once

#include "JucePlugin.h"


namespace Ade {

class Reverb: public JucePlugin<juce::dsp::Reverb> {
public:
    std::string name = "Reverb";

    Reverb() {
        parameters.push_back("wetLevel");
        parameters.push_back("dryLevel");
        parameters.push_back("roomSize");
        parameters.push_back("damping");
        parameters.push_back("width");
    }

    PluginParam getParam(std::string paramName) {
        auto params = processor.getParameters();
        PluginParam p;
        if(paramName == "wetLevel") {
            p.value = params.wetLevel;
            p.min = 0;
            p.max = 1;
            return p;
        }
        if(paramName == "dryLevel") {
            p.value = params.dryLevel;
            p.min = 0;
            p.max = 1;
            return p;
        }
        if(paramName == "roomSize") {
            p.value = params.roomSize;
            p.min = 0;
            p.max = 1;
            return p;
        }
        if(paramName == "damping") {
            p.value = params.damping;
            p.min = 0;
            p.max = 1;
            return p;
        }
        if(paramName == "width") {
            p.value = params.width;
            p.min = 0;
            p.max = 1;
            return p;
        }

        p.value = -1;
        return p;
    }

    void setParam(std::string paramName, float value) {
        auto params = processor.getParameters();
        if(paramName == "wetLevel") {
            params.wetLevel = value;
        }
        if(paramName == "dryLevel") {
            params.dryLevel = value;
        }
        if(paramName == "damping") {
            params.damping = value;
        }
        if(paramName == "roomSize") {
            params.roomSize = value;
        }
        if(paramName == "width") {
            params.width = value;
        }

        processor.setParameters(params);
    }
};

}