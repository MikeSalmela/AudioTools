/*
  Class for manipulating Wav audio data
*/


#include "Wav.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <math.h>
#include "../MyException.h"


Wav::Wav(){

}

Wav::Wav(const string &filename){
  read_file(filename);
}

// Change the raw data portion
void Wav::changeDATA(vector<float> &&newData){
  data1_ = newData;
}

// Write a comma seperated value file of the audio data
void Wav::writeCSV(const string& filename, int channel){
  ofstream out(filename);
  if(channel == 1){
    for(const float &f : data1_){
      out << f << " , ";
    }
  }
  else{
    for(const float &f : data2_){
      out << f << " , ";
    }
  }
  out.close();
}

// getters for WAV information
int32_t Wav::get_sampleRate(){
  return fmt_.SampleRate;
}
int16_t Wav::get_channelCount(){
  return fmt_.NumChannels;
}

// Return the RAW data portion of the wav file
vector<float> Wav::get_RAW_data(int channel) const{
  if(channel == 1)  return data1_;
  else return data2_;
}


// Reads the given file and fills memeber variables acordingly
void Wav::read_file(const string &filename){
  // get the full file as a string
  ifstream file(filename, ios_base::binary);
  try{
    readRIFF(file);
    readFMT(file);
    readSubChunk2(file);
  }
  catch (MyException &e){
    cerr << e.get_msg() << endl;
  }
  file.close();
}

void Wav::writeWAV(const string& filename, int bitsPerSample){
  ofstream out(filename, ofstream::binary);
  bitsPerSample = 16;
  writeRIFF(out);
  writeFMT(out, bitsPerSample);
  writeSubChunk2(out, bitsPerSample);
}

// fills the RIFF struct with data starting from the string iterator
void Wav::readRIFF(ifstream& in){

  read_ifsteam(in, riff_.ChunkID, big_endian);
  read_ifsteam(in, riff_.ChunkSize);
  read_ifsteam(in, riff_.Format, big_endian);

  // RIFF header should begin with "RIFF", and the format should be WAV
  if(riff_.ChunkID != RIFF_VALUE || riff_.Format != WAV_VALUE){
    throw MyException("Not a correct wav file");
  }
}

void Wav::writeRIFF(ofstream& out){
  write_ofsteam(out, riff_.ChunkID, big_endian);
  write_ofsteam(out, riff_.ChunkSize, big_endian);
  write_ofsteam(out, riff_.Format, big_endian);
}

// fills the FMT struct with data starting from the string iterator
void Wav::readFMT(ifstream& in){

  read_ifsteam(in, fmt_.SubChunk1ID, big_endian);
  read_ifsteam(in, fmt_.SubChunk1Size);
  read_ifsteam(in, fmt_.AudioFormat);
  read_ifsteam(in, fmt_.NumChannels);
  read_ifsteam(in, fmt_.SampleRate);
  read_ifsteam(in, fmt_.ByteRate);
  read_ifsteam(in, fmt_.BlockAlign);
  read_ifsteam(in, fmt_.BitsPerSample);

  if(fmt_.SubChunk1ID != FMT_VALUE){
    throw( MyException("not FMT"));
  }
}

void Wav::writeFMT(ofstream& out, int16_t bitsPerSample){
    write_ofsteam(out, fmt_.SubChunk1ID, big_endian);
    fmt_.SubChunk1Size = 16;  //Skip extra chunks
    write_ofsteam(out, fmt_.SubChunk1Size);
    write_ofsteam(out, fmt_.AudioFormat);
    write_ofsteam(out, fmt_.NumChannels);
    fmt_.ByteRate = (fmt_.SampleRate*bitsPerSample*fmt_.NumChannels)/8;
    write_ofsteam(out, fmt_.SampleRate);
    write_ofsteam(out, fmt_.ByteRate);
    fmt_.BlockAlign = (bitsPerSample* fmt_.NumChannels/8);
    write_ofsteam(out, fmt_.BlockAlign);
    write_ofsteam(out, bitsPerSample);
}

// Reads and saves the SubChunk2 data. Returns true when the ID is correct
void Wav::readSubChunk2(ifstream& in){

  read_ifsteam(in, SubChunk2ID_, big_endian);
  if(SubChunk2ID_ != DATA_VALUE){
    findDATA(in);
  }

  read_ifsteam(in, SubChunk2Size_);
  parseRAWdata(in);

}

void Wav::writeSubChunk2(ofstream& out, int16_t bitsPerSample){
  write_ofsteam(out, SubChunk2ID_, big_endian);
  SubChunk2Size_ = (data1_.size()*bitsPerSample*fmt_.NumChannels)/8;
  write_ofsteam(out, SubChunk2Size_);
  int t = 0;
  if(fmt_.BitsPerSample == 8) t = 127;
  long max_size = pow(2, bitsPerSample -1);
  for(const float &f : data1_){
    int16_t value = ((int16_t) (f*max_size + t));
    write_ofsteam(out, value);
  }
}

// Reads the raw data from ifstream. Data is normalized between -1 and 1
template<typename T>
void Wav::fillRAWdata(ifstream& in, T variable){

  // calculates the value needed for the normalization
  float f = pow(2, sizeof(variable)*8 - 1);
  // b is needed for 8-bit data only. 8-bit samples are unsigned and
  // need to be normalized differently to get values between -1 and 1
  float b = 0;
  if(sizeof(variable) == 1) {
    b = 128;
  }

  // samples per channel
  long samples = SubChunk2Size_/(sizeof(variable)*fmt_.NumChannels);

  data1_.resize(samples);
  if(fmt_.NumChannels == 2){  // in case of stereo
    data2_.resize(samples);
  }

  // Do the normalization and save data in the vector
  for(size_t i = 0; i < samples; ++i){
    read_ifsteam(in, variable);
    data1_[i] = ((variable-b)/f);

    if(fmt_.NumChannels == 2){  // for stereo
      read_ifsteam(in, variable);
      data2_[i] = ((variable-b)/f);
    }
  }
}

// Selects the correct variable for the fillRAWdata function
void Wav::parseRAWdata(ifstream& in){
  switch (fmt_.BitsPerSample) {
    case 8: {
      uint8_t c = 0;
      fillRAWdata(in, c);
      break;
    }
    case 16: {
      int16_t i = 0;
      fillRAWdata(in, i);
      break;
    }
    case 32:  {
      int32_t i = 0;
      fillRAWdata(in, i);
      break;
    }
  }
}


// Fills the parameter variable with data from ifstream
template<typename T>
ifstream& Wav::read_ifsteam(ifstream& in, T& variable, Endian endian){
  if(!in.read(reinterpret_cast<char*>(&variable), sizeof(T))){
    throw(MyException("Error reading the file"));
  }
  // Reverse the bytes for big endian
  if(endian == big_endian){
    uint8_t *pvariable = reinterpret_cast<unsigned char*>(&variable);
    reverse(pvariable, pvariable + sizeof(T));
  }
}

// Write the variable to the ofstream out
template<typename T>
ostream& Wav::write_ofsteam(ofstream& out, T& variable, Endian endian){
  if(endian == big_endian){
    uint8_t *pvariable = reinterpret_cast<unsigned char*>(&variable);
    reverse(pvariable, pvariable + sizeof(T));
  }

  if(!out.write(reinterpret_cast<char*>(&variable), sizeof(T))){
    throw(MyException("Error writing the file"));
  }
}

// Find the wav data
void Wav::findDATA(ifstream& in){
  char c = 0;
  while(c != 'd'){
    read_ifsteam(in, c);
  }
  read_ifsteam(in, c);
  if(c != 'a'){
    findDATA(in);
    return;
  }
  read_ifsteam(in, c);
  if(c != 't')  {
    findDATA(in);
    return;
  }
  read_ifsteam(in, c);
  if(c != 'a')  {
    findDATA(in);
  }
  SubChunk2ID_ = DATA_VALUE;
}
