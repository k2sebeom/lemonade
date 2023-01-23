

#pragma once


#include "JuceHeader.h"


namespace Ade {

enum ChannelLayout {
    INTERLEAVED, PLANAR
};

template <typename T> juce::AudioBuffer<T> floatArrayToJuceBuffer(T* data, unsigned int nChannels, unsigned int nFrames, ChannelLayout layout = INTERLEAVED) {
    juce::AudioBuffer<T> buffer(nChannels, nFrames);

    switch(layout) {
    case ChannelLayout::INTERLEAVED:
        for (unsigned int i = 0; i < nChannels; i++) {
            T *channelBuffer = buffer.getWritePointer(i);
            // We're de-interleaving the data here, so we can't use copyFrom.
            for (unsigned int j = 0; j < nFrames; j++) {
                channelBuffer[j] = data[j * nChannels + i];
            }
        }
        break;
    
    case ChannelLayout::PLANAR:
        for (unsigned int i = 0; i < nChannels; i++) {
            buffer.copyFrom(i, 0, data + (nFrames * i), nFrames);
        }
        break;
    }

    return buffer;
}
}