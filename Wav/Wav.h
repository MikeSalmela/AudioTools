/*
  Class for manipulating data in a wav file
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

  // Change the raw data portion
  void changeDATA(const vector<float> &newData);

  // Reads the data in given file and saves the parsed data
  void read_file(const string &filename);

  // Write out the normalized audio data
  void writeCSV(const string& filename, int channel = 1);

  // getters for WAV information
  int32_t get_sampleRate();
  int16_t get_channelCount();
  vector<float> const get_RAW_data(int channel = 1);

private:
  // the RIFF header
  struct RIFF riff_;
  //the FMT wav header
  struct FMT fmt_;
  //the SubChunk2
  int32_t SubChunk2ID_;    // this should equal WAV_DATA
  int32_t SubChunk2Size_;  // length of the audio data
  // the raw audio data
  vector<float> data1_; //Channel one
  vector<float> data2_; //Channel two (if stereo)


  // read the RIFF data with iterator and fill the struct. iterator will be moved
  void fillRIFF(ifstream &in);
  // read the FMT data with iterator and fill the struct. iterator will be moved
  void fillFMT(ifstream& in);
  // Fill the SubChunk2
  void fillSubChunk2(ifstream& in);
  // Fills the data_ vectors
  template<typename T>
  void fillRAWdata(ifstream& in, T variable);
  void parseRAWdata(ifstream& in);

  void findDATA(ifstream& in);

  // Fill the T variable with data from ifstream in
  template<typename T>
  ifstream& read_ifsteam(ifstream& in, T& variable, Endian endian = little_endian);

  string write_N_bits(int32_t in, Endian endian, int n);

};



#endif
