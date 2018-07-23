
#include "Audio.h"
#include "../../MyException.h"
#include <cmath>

const double PI = 3.14159265;



//Returns a vector with raw data sine wave.
std::vector<float> makeSine(int frequency, int sampleRate, int length){

    if(frequency*2 > sampleRate) {
        throw MyException("Frequency can't be higher than sample rate/2");
    }
    
    std::vector<float> sineWave(length);
    for(int i = 0; i < length; ++i){
        sineWave[i] = sin(i*((float) frequency/sampleRate)*PI);
    }
    return sineWave;

}


