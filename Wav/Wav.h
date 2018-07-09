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
  void changeDATA(vector<float> &&newData);

  // Reads the data in given file and saves the parsed data
  void read_file(const string &filename);

  // Write out the normalized audio data
  void writeCSV(const string& filename, int channel = 1);

  void writeWAV(const string& filename, int bitsPerSample = 16);

  // getters for WAV information
  int32_t get_sampleRate();
  int16_t get_channelCount();
  vector<float> get_RAW_data(int channel = 1) const;

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
  void readRIFF(ifstream &in);
  // read the FMT data with iterator and fill the struct. iterator will be moved
  void readFMT(ifstream& in);
  // Fill the SubChunk2
  void readSubChunk2(ifstream& in);
  // Fills the data_ vectors
  template<typename T>
  void fillRAWdata(ifstream& in, T variable);
  void parseRAWdata(ifstream& in);
  void findDATA(ifstream& in);

  //  Methods for writing a wav file
  void writeRIFF(ofstream& out);
  void writeFMT(ofstream& out, int16_t bitsPerSample);
  void writeSubChunk2(ofstream& out, int16_t bitsPerSample);

  // Fill the T variable with data from ifstream in
  template<typename T>
  ifstream& read_ifsteam(ifstream& in, T& variable, Endian endian = little_endian);


  template<typename T>
  ostream& write_ofsteam(ofstream& out, T& variable, Endian endian = little_endian);
};



#endif
