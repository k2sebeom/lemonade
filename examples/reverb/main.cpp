#include <iostream>
#include "lemonade.h"
#include "plugins/Reverb.h"


int main(int argc, const char** argv) {
    Ade::Chain c;
    c.Hello();
    Ade::Reverb reverb;
    std::cout << reverb.name << std::endl;
    return 0;
}