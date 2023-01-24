#include <iostream>
#include "lemonade.h"
#include "ade_plugins/Reverb.h"
#include <sndfile.hh>
#include <string>
#include "ExternalPlugin.h"
#include <thread>

#include <iostream>
#include "lemonade.h"
#include "ade_plugins/Reverb.h"
#include <sndfile.hh>
#include <string>


int main(int argc, const char** argv) {
    std::string pathToPluginFile = "/Library/Audio/Plug-Ins/VST3/TAL-Reverb-2.vst3";
    std::string filePath = argv[1];
    std::string outPath = argv[2];
    SndfileHandle reader(filePath);

    Ade::Chain chain;
    Ade::ExternalPlugin plugin(pathToPluginFile);
    std::cout << plugin.name << std::endl;
    chain.push(&plugin);

    plugin.showEditor();

    SndfileHandle writer(outPath, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT, reader.channels(), reader.samplerate());

    float frames[2048];
    sf_count_t framesRead = 1024;
    while(framesRead > 0) {
        framesRead = reader.readf(frames, 1024);
        chain.process(frames, reader.samplerate(), reader.channels(), framesRead);
        writer.writef(frames, framesRead);
    }
    return 0;
}
