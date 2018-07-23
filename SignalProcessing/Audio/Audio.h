//Functions for creating raw audio data samples e.g. Sine wave
#ifndef AUDIO_H
#define AUDIO_H

#include <vector>

//Returns a vector with raw data sine wave.
std::vector<float> makeSine(int frequency, int sampleRate, int length);





#endif