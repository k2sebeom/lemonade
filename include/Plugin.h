#pragma once

#include <string>

namespace Ade {

class Plugin {
public:
    std::string name;

    virtual ~Plugin() {};

    virtual void process(float *data, double sampleRate, unsigned int nChannels, unsigned int nFrames) {};
};
}