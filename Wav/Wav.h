/*
  Class for manipulating data in a wav file
*/

#ifndef WAV_H
#define WAV_H

#include <vector>
#include <string>
#include <fstream>
#include "WavDefine.h"



class Wav{
public:


  Wav();
  Wav(const std::string &filename);
  // Create a wav object with given data
  Wav(const std::vector<float> &data, int32_t sampleRate);

  // Change the raw data portion
  void changeDATA(std::vector<float> &&newData);

  // Reads the data in given file and saves the parsed data
  void read_file(const std::string &filename);

  // Write out the normalized audio data
  void writeCSV(const std::string& filename, int channel = 1);

  void writeWAV(const std::string& filename, int bitsPerSample = 16);

  // getters for WAV information
  int32_t get_sampleRate();
  int16_t get_channelCount();
  std::vector<float> get_RAW_data(int channel = 1) const;

private:
  // the RIFF header
  struct RIFF riff_;
  //the FMT wav header
  struct FMT fmt_;
  //the SubChunk2
  int32_t SubChunk2ID_;    // this should equal WAV_DATA
  int32_t SubChunk2Size_;  // length of the audio data
  // the raw audio data
  std::vector<float> data1_; //Channel one
  std::vector<float> data2_; //Channel two (if stereo)


  // read the RIFF data with iterator and fill the struct. iterator will be moved
  void readRIFF(std::ifstream &in);
  // read the FMT data with iterator and fill the struct. iterator will be moved
  void readFMT(std::ifstream& in);
  // Fill the SubChunk2
  void readSubChunk2(std::ifstream& in);
  // Fills the data_ vectors
  template<typename T>
  void fillRAWdata(std::ifstream& in, T variable);
  void parseRAWdata(std::ifstream& in);
  void findDATA(std::ifstream& in);

  //  Methods for writing a wav file
  void writeRIFF(std::ofstream& out);
  void writeFMT(std::ofstream& out, int16_t bitsPerSample);
  void writeSubChunk2(std::ofstream& out, int16_t bitsPerSample);

  // Fill the T variable with data from ifstream in
  template<typename T>
  std::ifstream& read_ifsteam(std::ifstream& in, T& variable, Endian endian = little_endian);


  template<typename T>
  std::ostream& write_ofsteam(std::ofstream& out, T& variable, Endian endian = little_endian);
};



#endif
