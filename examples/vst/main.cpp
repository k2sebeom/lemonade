#include <iostream>
#include "lemonade.h"
#include "ade_plugins/Reverb.h"
#include <sndfile.hh>
#include <string>
#include "ExternalPlugin.h"
#include <thread>


int main(int argc, const char** argv) {
    std::string pathToPluginFile = "/Library/Audio/Plug-Ins/VST3/TAL-Reverb-2.vst3";
    Ade::ExternalPlugin plugin(pathToPluginFile);

    plugin.showEditor();
    int f;
    std::cin >> f;
    return 0;
}