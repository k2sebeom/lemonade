

#pragma once


#include "JuceHeader.h"


namespace Ade {

enum ChannelLayout {
    INTERLEAVED, PLANAR
};

template <typename T> juce::AudioBuffer<T> arrayToJuceBuffer(T* data, unsigned int nChannels, unsigned int nFrames, ChannelLayout layout = INTERLEAVED) {
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

template <typename T> T* juceBufferToArray(juce::AudioBuffer<T> buffer, ChannelLayout layout = INTERLEAVED) {
    unsigned int nChannels = buffer.getNumChannels();
    unsigned int nFrames = buffer.getNumSamples();

    T *array = new T[nChannels * nFrames];

    switch(layout) {
    case ChannelLayout::INTERLEAVED:
        for (unsigned int i = 0; i < nChannels; i++) {
            const T *channelBuffer = buffer.getReadPointer(i);
            // We're interleaving the data here, so we can't use copyFrom.
            for (unsigned int j = 0; j < nFrames; j++) {
                array[j * nChannels + i] = channelBuffer[j];
            }
         }
        break;
    case ChannelLayout::PLANAR:
        for (unsigned int i = 0; i < nChannels; i++) {
            const T *channelBuffer = buffer.getReadPointer(i);
            for (unsigned int j = 0; j < nFrames; j++) {
                array[i * nFrames + j] = channelBuffer[j];
            }
        }
        break;
    }

    return array;
}
}