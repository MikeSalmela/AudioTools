#ifndef FIR_H
#define FIR_H

#include <string>
#include <vector>

enum FilterType{
  LOWPASS, HIGHPASS
};

class FIR{

public:

  FIR(int n = 10, float fc = 0.25, FilterType type = LOWPASS);

  // filters and returns the given data
  std::vector<float> filter(const std::vector<float> &input_data);


private:
  // Coefficents for this FIR filter
  std::vector<float> impulseResponse_;

  // Returns the coefficents of a wanted filter
  std::vector<float> designFilter(int n, float fc, FilterType type = LOWPASS);


  // Returns a Hamming window with n + 1 terms
  std::vector<float> designHamming(int n);


};


// Normalized sinc function
float sinc(float x);

#endif
