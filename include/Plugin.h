#pragma once

#include <string>

namespace Ade {

class Plugin {
public:
    std::string name;

    virtual ~Plugin() {};

    // virtual void prepare();

    // virtual void process(float *data);
};
}