#ifndef LEMONADE_H

#define LEMONADE_H

#include "JuceHeader.h"
#include "Plugin.h"
#include <vector>

namespace Ade {

class Chain {
public:
    Chain();
    ~Chain();

    void push(Ade::Plugin *plugin);
    void removeAt(int i);
    void process(float *data, double sampleRate, unsigned int nChannels, unsigned int nFrames);

    int size();

    Ade::Plugin *at(int i);

private:
    std::vector<Ade::Plugin*> plugins;
};

}

#endif