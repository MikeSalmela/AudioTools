#ifndef FIR_H
#define FIR_H

#include <string>
#include <vector>

using namespace std;

class FIR{

public:
  FIR();

  // filters and returns the given data
  vector<uint8_t> filter(vector<uint8_t> input_data);


    // Returns the coefficents of a wanted lowpass filter
    vector<float> designLowpass(int n, float fc);

private:
  // Coefficents for this FIR filter
  vector<float> filterCoefficents_;



};


// Normalized sinc function
float sinc(float x);

#endif
