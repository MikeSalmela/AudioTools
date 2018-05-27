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

  // Return the RAW data portion of the wav file
  vector<char> const get_RAW_data();

private:
  // the RIFF header
  struct RIFF riff_;
  //the FMT wav header
  struct FMT fmt_;
  //the SubChunk2
  uint32_t SubChunk2ID_;    // this should equal WAV_DATA
  uint32_t SubChunk2Size_;  // length of the audio data
  // the raw audio data
  vector<char> data_;


  // read the RIFF data with iterator and fill the struct. iterator will be moved
  void fillRIFF(string::iterator &it);
  // read the FMT data with iterator and fill the struct. iterator will be moved
  void fillFMT(string::iterator &it);
  // Fill the SubChunk2
  bool fillSubChunk2(string::iterator &it);
  // read all the data in a wav file starting from the iterator. iterator will be moved
  void parse_data(string::iterator &it, string &data);

};

// read 32 bits from the given string iterator
// iterator will be moved 32 bits
uint32_t read_32bits(string::iterator &it, Endian endian);
// read 16 bits from the given string iterator
// iterator will be moved 16 bits
uint16_t read_16bits(string::iterator &it, Endian endian);

string file_to_string(const string &filename);



#endif
