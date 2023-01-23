#include "lemonade.h"

void Ade::Chain::push(Ade::Plugin *plugin) {
    plugins.push_back(plugin);
}

void Ade::Chain::removeAt(int i) {
    if(i < plugins.size()) {
        plugins.erase(plugins.begin() + i);
    }
}

Ade::Plugin* Ade::Chain::at(int i) {
    if(i < plugins.size()) {
        return plugins.at(i);
    }
    return nullptr;
}

int Ade::Chain::size() {
    return plugins.size();
}

void Ade::Chain::process(float *data, double sampleRate, unsigned int nChannels, unsigned int nFrames) {
    for(int i = 0; i < plugins.size(); i++) {
        plugins.at(i)->process(data, sampleRate, nChannels, nFrames);
    }
}

Ade::Chain::Chain() {

}

Ade::Chain::~Chain() {

}