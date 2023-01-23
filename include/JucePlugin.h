#include "JuceHeader.h"
#include "Plugin.h"



namespace Ade {
template <typename ProcessorType> class JucePlugin: public Plugin {
public:
    virtual ~JucePlugin(){};
    ProcessorType processor;
};
}