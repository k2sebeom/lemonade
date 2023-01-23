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