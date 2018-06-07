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

private:
  // Coefficents for this FIR filter
  vector<double> filterCoefficents_;


};

#endif
