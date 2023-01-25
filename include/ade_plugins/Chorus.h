#pragma once

#include "JucePlugin.h"
#include <map>


namespace Ade {

class Chorus: public JucePlugin<juce::dsp::Chorus<float>> {
public:
    std::string name = "Chorus";

    Chorus() {
        parameters.push_back("rate");
        parameters.push_back("depth");
        parameters.push_back("centralDelay");
        parameters.push_back("feedback");
        parameters.push_back("mix");

        chorusParams.insert({"rate", 1});
        chorusParams.insert({"depth", 0.25});
        chorusParams.insert({"feedback", 0.0});
        chorusParams.insert({"centralDelay", 7.0});
        chorusParams.insert({"mix", 0.5});

        processor.setRate(1.0);
        processor.setDepth(0.25);
        processor.setCentreDelay(7.0);
        processor.setMix(0.5);
        processor.setFeedback(0.0);
    }

    PluginParam getParam(std::string paramName) {
        PluginParam p;
        auto i = chorusParams.find(paramName);
        if(i == chorusParams.end()) {
            p.value = -1;
            return p;
        }
        p.value = i->second;

        if(paramName == "rate") {
            p.min = 0;
            p.max = 100;
            return p;
        }
        if(paramName == "depth") {
            p.min = 0;
            p.max = 1;
            return p;
        }
        if(paramName == "centralDelay") {
            p.min = 0;
            p.max = 100;
            return p;
        }
        if(paramName == "feedback") {
            p.min = -1;
            p.max = 1;
            return p;
        }
        if(paramName == "mix") {
            p.min = 0;
            p.max = 1;
            return p;
        }

        p.value = -1;
        return p;
    }

    void setParam(std::string paramName, float value) {
        if(paramName == "rate") {
            processor.setRate(value);
        }
        if(paramName == "depth") {
            processor.setDepth(value);
        }
        if(paramName == "centralDelay") {
            processor.setCentreDelay(value);
        }
        if(paramName == "feedback") {
            processor.setFeedback(value);
        }
        if(paramName == "mix") {
            processor.setMix(value);
        }
        chorusParams[paramName] = value;
    }

private:
    std::map<std::string, float> chorusParams;
};

}