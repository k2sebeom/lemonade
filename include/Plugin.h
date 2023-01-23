#pragma once

#include <string>
#include <vector>

namespace Ade {

class PluginParam {
public:
    float value;
    float min;
    float max;
};

class Plugin {
public:
    std::string name;

    virtual ~Plugin() {};

    virtual void process(float *data, double sampleRate, unsigned int nChannels, unsigned int nFrames) {};

    std::vector<std::string> parameters;

    virtual PluginParam getParam(std::string paramName) { return PluginParam(); };
    virtual void setParam(std::string paramName, float value) {};
};
}