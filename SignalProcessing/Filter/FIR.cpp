#include "FIR.h"
#include <iostream>
#include <cmath>

#define PI 3.14159265


FIR::FIR(int n , float fc, FilterType type){

  vector<float> filter = designFilter(n, fc, type);
  vector<float> window = designHamming(n);

  for(size_t n = 0; n <= filter.size(); ++n){
    impulseResponse_.push_back(filter[n]*window[n]);
  }

}

vector<int16_t> FIR::filter(const vector<int16_t> &input_data){
  vector<int16_t> data(input_data.size());

  //
  for(size_t n = 0; n < input_data.size(); ++n){
    float temp = 0;
    for(size_t k = 0; k < impulseResponse_.size(); ++k){
      if(n - k >= 0){
          temp += impulseResponse_[k] * (float) input_data[n - k];
      }
    }
    //if(temp > x) temp = 255;
    //if (temp < 0) temp = 0;
    data[n] = ((int16_t) temp);
  }
  return data;
}


vector<uint8_t> FIR::filter(const vector<uint8_t> &input_data){
  vector<uint8_t> data(input_data.size());

  //
  for(size_t n = 0; n < input_data.size(); ++n){
    float temp = 0;
    for(size_t k = 0; k < impulseResponse_.size(); ++k){
      if(n - k >= 0){
          temp += impulseResponse_[k] * (float) input_data[n - k];
      }
    }
    if(temp > 255) temp = 255;
    if (temp < 0) temp = 0;
    data[n] = ((uint8_t) temp);
  }
  return data;
}

// Returns the coefficents of a filter
vector<float> FIR::designFilter(int n, float fc, FilterType type){

  vector<float> filter(n + 1);

  n = n/2;
  float coefficent = 0;
  int k = 0;

  for(int i = -n; i <= n; ++i){
    if(i != 0){
      coefficent = 2*fc*sinc(i*2*fc);
      if(type == HIGHPASS){
        coefficent *= -1;
      }
    }
    else{
      coefficent = 2*fc;
      if(type == HIGHPASS){
        coefficent = 1 - coefficent;
      }
    }
    filter[k] = coefficent;
    ++k;
  }
  return filter;
}



  // Returns a Hamming window with n + 1 terms
vector<float> FIR::designHamming(int n){
    vector<float> Hamming(n + 1);
    int k = 0;

    for(int i = -n/2; i <= n/2; ++i){
      ++k;
      Hamming[k] = (0.5 + 0.5*cos((2*PI*i)/n));
    }
    return Hamming;
}


// Normalized sinc function
float sinc(float x){
  return (sin(PI*x)/(PI*x));
}
