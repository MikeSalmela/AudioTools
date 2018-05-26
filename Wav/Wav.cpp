#include "Wav.h"
#include <iostream>
#include "../MyException.h"

// read 32 bits from the given string iterator
// iterator will be moved 32 bits
uint32_t read_32bits(string::iterator &it, Endian endian);
// read 16 bits from the given string iterator
// iterator will be moved 16 bits
uint16_t read_16bits(string::iterator &it, Endian endian);

string file_to_string(const string &filename);

Wav::Wav(){

}

Wav::Wav(const string &filename){
  read_data(filename);
}

// Reads the given file and fills memeber variables acordingly
void Wav::read_data(const string &filename){
  // get the full file as a string
  string file;
  try{
    file = file_to_string(filename);
    string::iterator it = file.begin();
    // parse the header data
    parse_data(it);
  }
  catch (MyException &e){
    cerr << e.get_msg() << endl;
  }
}

// fills the RIFF struct with data starting from the string iterator
void Wav::fillRIFF(string::iterator &it){
  riff_.ChunkID = read_32bits(it, big_endian);
  riff_.ChunkSize = read_32bits(it, little_endian);
  riff_.Format = read_32bits(it, big_endian);
  // RIFF header should begin with "RIFF", and the format should be WAV
  if(riff_.ChunkID != RIFF_VALUE || riff_.Format != WAV_VALUE){
    throw MyException("Not a correct wav file");
  }
}


// fills the FMT struct with data starting from the string iterator
void Wav::fillFMT(string::iterator &it){
  fmt_.SubChunk1ID = read_32bits(it, big_endian);
  fmt_.SubChunk1Size = read_32bits(it, little_endian);
  fmt_.AudioFormat = read_16bits(it, little_endian);
  fmt_.NumChannels = read_16bits(it, little_endian);
  fmt_.SampleRate = read_32bits(it, little_endian);
  fmt_.ByteRate = read_32bits(it, little_endian);
  fmt_.BlockAlign = read_16bits(it, little_endian);
  fmt_.BitsPerSample = read_16bits(it, little_endian);

  // if not PCM
  if(fmt_.SubChunk1Size != 16){
    throw(MyException("Subchunk1 not 16 bytes"));
  }
}


// read 32 bits from the given string iterator
// iterator will be moved 32 bits
uint32_t read_32bits(string::iterator &it, Endian endian){
  // read the data in the iterator for 4 bytes (char)
  uint8_t read_data[4];
  for(int i = 0; i < 4; ++i){
    read_data[i] = *it;
    ++it;
  }
  uint32_t data = 0;
  // Shift the bits to form a 32 bit variable
  if(endian == little_endian){
    data = ((read_data[0]) | read_data[1] << 8 |
      read_data[2] << 16 | read_data[3] << 24);
  }
  else{
    data = ((read_data[0] << 24) | read_data[1] << 16 |
      read_data[2] << 8 | read_data[3]);
  }
  return data;

}
// read 16 bits from the given string iterator
// iterator will be moved 16 bits
uint16_t read_16bits(string::iterator &it, Endian endian){
  // read the data in the iterator for 4 bytes (char)
  uint8_t read_data[2];
  for(int i = 0; i < 2; ++i){
    read_data[i] = *it;
    ++it;
  }
  uint32_t data = 0;
  // Shift the bits to form a 16 bit variable
  if(endian == little_endian){
    data = ((read_data[0]) | read_data[1] << 8);
  }
  else{
    data = (read_data[0] << 8 | read_data[1]);
  }
  return data;
}


// Reads all the header information and fills Class variables
void Wav::parse_data(string::iterator &it){

  fillRIFF(it);
  fillFMT(it);
  SubChunk2ID_ = read_32bits(it, big_endian);
  SubChunk2Size_ = read_32bits(it, little_endian);
  if(SubChunk2ID_ != WAV_DATA){
    throw MyException("Subchunk2 not Wav");
  }
  for(int i = 0; i < SubChunk2Size_; ++i)
  {
    data_.push_back(*it);
    ++it;
  }
}

// Return the RAW data portion of the wav file
vector<char> const Wav::get_RAW_data(){
  return data_;
}

// Read the whole file into a string
string file_to_string(const string &filename){
  ifstream inputfile(filename);
  if(!inputfile){
    throw MyException("Invalid inputfile");
  }
  string fullFile, line;
  while(getline(inputfile, line)){
    fullFile += line;
  }
  inputfile.close();
  return fullFile;

}
