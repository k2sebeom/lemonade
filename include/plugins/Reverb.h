#include "JucePlugin.h"


namespace Ade {

class Reverb: public JucePlugin<juce::dsp::Reverb> {
public:
    std::string name = "Reverb";
};

}