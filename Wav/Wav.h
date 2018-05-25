/*
  Class for accessing data in a wav file
*/

#ifndef WAV_H
#define WAV_H

#include <vector>
#include <string>
#include <fstream>
#include "WavDefine.h"

using namespace std;

class Wav{
public:
  Wav();
  Wav(const string &filename);
  // Reads the data in given file and saves the parsed data
  void read_data(const string &filename);

private:
  // the RIFF header
  struct RIFF riff_;
  //the FMT wav header
  struct FMT fmt_;
  //the SubChunk2
  uint32_t SubChunk2ID_;
  uint32_t SubChunk2Size_;
  // the raw audio data
  vector<char> data_;
  // read the RIFF data and fill the struct
  void fillRIFF(string::iterator &it);
  // read the FMT data and fill the struct
  void fillFMT(string::iterator &it);

};

#endif
