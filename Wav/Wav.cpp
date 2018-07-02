/*
  Class for manipulating Wav audio data
*/


#include "Wav.h"
#include <iostream>
#include <algorithm>
#include "../MyException.h"


Wav::Wav(){

}

Wav::Wav(const string &filename){
  read_file(filename);
}

// Change the raw data portion
void Wav::changeDATA(const vector<float> &newData){
  data_ = newData;
}

// getters for WAV information
int32_t Wav::get_sampleRate(){
  return fmt_.SampleRate;
}
int16_t Wav::get_channelCount(){
  return fmt_.NumChannels;
}
// Return the RAW data portion of the wav file
vector<float> const Wav::get_RAW_data(){
  return data_;
}


// Reads the given file and fills memeber variables acordingly
void Wav::read_file(const string &filename){
  // get the full file as a string
  ifstream file(filename, ios_base::binary);
  try{
    fillRIFF(file);
    fillFMT(file);
    fillSubChunk2(file);
  }
  catch (MyException &e){
    cerr << e.get_msg() << endl;
  }
}

// fills the RIFF struct with data starting from the string iterator
void Wav::fillRIFF(ifstream& in){

  read_ifsteam(in, riff_.ChunkID, big_endian);
  read_ifsteam(in, riff_.ChunkSize);
  read_ifsteam(in, riff_.Format, big_endian);

  // RIFF header should begin with "RIFF", and the format should be WAV
  if(riff_.ChunkID != RIFF_VALUE || riff_.Format != WAV_VALUE){
    throw MyException("Not a correct wav file");
  }
}


// fills the FMT struct with data starting from the string iterator
void Wav::fillFMT(ifstream& in){

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


// Reads and saves the SubChunk2 data. Returns true when the ID is correct
void Wav::fillSubChunk2(ifstream& in){

  read_ifsteam(in, SubChunk2ID_, big_endian);
  read_ifsteam(in, SubChunk2Size_);
  for(long i = 0;
      i < ((SubChunk2Size_)*(fmt_.BitsPerSample)*(fmt_.NumChannels))/8; ++i){
    char temp;
    read_ifsteam(in, temp);
  }
  
}


// Fills the parameter variable with data from ifstream
template<typename T>
void Wav::read_ifsteam(ifstream& in, T& variable, Endian endian){
  if(!in.read(reinterpret_cast<char*>(&variable), sizeof(T))){
    throw(MyException("Error reading the file"));
  }

  // Reverse the bytes for big endian
  if(endian == big_endian){
    uint8_t *pvariable = reinterpret_cast<unsigned char*>(&variable);
    reverse(pvariable, pvariable + sizeof(T));
  }
}



// Returns a string with the in data written accordingly to the Endian
string write_N_bits(int32_t in, Endian endian, int n){
  char bytes[n];
  string bitString;

  if(endian == little_endian){
    for(int i = 0; i < n; ++i){
      bytes[i] = (in >> i*8) & 0xFF;
    }
  }
  else{
    for(int i = n; i > 0; --i){
      bytes[n - i] = (in >> (i-1)*8) & 0xFF;
    }
  }
  for(int i = 0; i < n; ++i){
    bitString += bytes[i];
  }
  return bitString;
}
