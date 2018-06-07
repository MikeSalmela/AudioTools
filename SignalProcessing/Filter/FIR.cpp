#include "FIR.h"
#include <iostream>

FIR::FIR(){
  // FIR filter with 11 elements and cutoff at 0.25 for test purposes
  filterCoefficents_ = { -0.0039,    0.0000,    0.0321,    0.1167,    0.2207,    0.2687,    0.2207,    0.1167,    0.0321,    0.0000,   -0.0039 };

}

vector<uint8_t> FIR::filter(vector<uint8_t> input_data){
  vector<uint8_t> data;

  for(size_t n = 0; n < input_data.size(); ++n){
    float temp = 0;
    for(size_t k = 0; k < filterCoefficents_.size(); ++k){
      if(n - k >= 0){
          temp += (float) filterCoefficents_[k] * (float) input_data[n - k];
      }
    }
    if(temp > 255) temp = 255;
    if (temp < 0) temp = 0;
    data.push_back((uint8_t) temp);
  }
  return data;
}
