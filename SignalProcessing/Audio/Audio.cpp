
#include "Audio.h"
#include "../../MyException.h"
#include <cmath>

const double PI = 3.14159265;


//Returns a vector with raw data sine wave.
std::vector<float> makeSine(float frequency, float sampleRate, int length){

    if(frequency*2 > sampleRate) {
        throw MyException("Frequency can't be higher than samplerate/2");
    }
    
    std::vector<float> sineWave(length);

    for(int i = 0; i < length; ++i){
        sineWave[i] = sin((frequency*i*2*PI)/sampleRate)*0.999;
    }
    return sineWave;

}


