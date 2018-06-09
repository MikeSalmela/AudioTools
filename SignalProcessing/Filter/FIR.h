#ifndef FIR_H
#define FIR_H

#include <string>
#include <vector>

using namespace std;

class FIR{

public:

  FIR(int n = 10, float fc = 0.25);

  // filters and returns the given data
  vector<uint8_t> filter(const vector<uint8_t> &input_data);
  vector<int16_t> filter(const vector<int16_t> &input_data);


private:
  // Coefficents for this FIR filter
  vector<float> impulseResponse_;

  // Returns the coefficents of a wanted lowpass filter
  vector<float> designLowpass(int n, float fc);

  // Returns a Hamming window with n + 1 terms
  vector<float> designHamming(int n);


};


// Normalized sinc function
float sinc(float x);

#endif
