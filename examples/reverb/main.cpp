#include <iostream>
#include "lemonade.h"
#include "ade_plugins/Reverb.h"
#include <sndfile.hh>
#include <string>


int main(int argc, const char** argv) {
    std::string filePath = argv[1];
    std::string outPath = argv[2];
    SndfileHandle reader(filePath);

    Ade::Reverb reverb;
    std::cout << reverb.name << std::endl;

    SndfileHandle writer(outPath, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT, reader.channels(), reader.samplerate());

    reverb.setParam("wetLevel", 0.7);
    reverb.setParam("roomSize", 0.7);

    for(int i = 0; i < reverb.parameters.size(); i++) {
        std::string paramName = reverb.parameters.at(i);
        std::cout <<  paramName << ": " << reverb.getParam(paramName).value << std::endl;
    }

    reverb.prepare(reader.samplerate(), reader.channels(), 1024);
    float frames[2048];
    sf_count_t framesRead = 1024;
    while(framesRead > 0) {
        framesRead = reader.readf(frames, 1024);
        reverb.process(frames, reader.samplerate(), reader.channels(), framesRead);
        writer.writef(frames, framesRead);
    }
    return 0;
}