/*
  Class for manipulating Wav audio data
*/


#include "Wav.h"
#include <iostream>
#include "../MyException.h"


Wav::Wav(){

}

Wav::Wav(const string &filename){
  read_data(filename);
}

// Change the raw data portion
void Wav::changeDATA(const vector<uint8_t> &newData){
  data_ = newData;
}

// Write a wav file from the Wav object
void Wav::writeWAV(const string& fileName){
  ofstream outPutStream(fileName);
  outPutStream << getWAV();

}


// getters for WAV information
uint32_t Wav::get_sampleRate(){
  return fmt_.SampleRate;
}
uint16_t Wav::get_channelCount(){
  return fmt_.NumChannels;
}




string Wav::getWAV(){
  string WAVFile;
  WAVFile += getRIFF();
  WAVFile += getFMT();
  WAVFile += write32Bits(SubChunk2ID_, big_endian);
  WAVFile += write32Bits(SubChunk2Size_, little_endian);
  for(uint8_t c : data_){
    WAVFile += c;
  }

  return WAVFile;
}

// Reads the given file and fills memeber variables acordingly
void Wav::read_data(const string &filename){
  // get the full file as a string
  string file;
  try{
    file = file_to_string(filename);
    string::iterator it = file.begin();
    // parse the header data
    parse_data(it, file);
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


  if(fmt_.SubChunk1ID != FMT_VALUE){
    throw( MyException("not FMT"));
  }
}

// Reads and saves the SubChunk2 data. Returns true when the ID is correct
bool Wav::fillSubChunk2(string::iterator &it){
  SubChunk2ID_ = read_32bits(it, big_endian);

  // check ID
  if(SubChunk2ID_ != DATA_VALUE) return false;

  SubChunk2Size_ = read_32bits(it, little_endian);
  for(int i = 0; i < SubChunk2Size_; ++i)
  {
    data_.push_back(*it);
    ++it;
  }
  return true;


}




// Reads all the header information and fills Class variables
void Wav::parse_data(string::iterator &it, string &data){

  fillRIFF(it);
  fillFMT(it);

  // the Data ID is usually here, but if not, find it in the file
  if(!fillSubChunk2(it)){
    size_t foundAt = data.find("data");
    it = data.begin();
    for(size_t i = 0; i < foundAt; ++i){
      ++it;
    }
    if(!fillSubChunk2(it)){
      throw MyException("Data ID not found");
    }
  }

}

// Return the RAW data portion of the wav file
vector<uint8_t> const Wav::get_RAW_data(){
  return data_;
}

// Create a string from the Wav RIFF header
string Wav::getRIFF(){
  string RIFF;
  RIFF += write32Bits(riff_.ChunkID, big_endian);
  RIFF += write32Bits(riff_.ChunkSize, little_endian);
  RIFF += write32Bits(riff_.Format, big_endian);

  return RIFF;
}

// Create a string from the Wav FMT header
string Wav::getFMT(){
  string FMT;
  FMT += write32Bits(fmt_.SubChunk1ID, big_endian);
  FMT += write32Bits(fmt_.SubChunk1Size, little_endian);
  FMT += write16Bits(fmt_.AudioFormat, little_endian);
  FMT += write16Bits(fmt_.NumChannels, little_endian);
  FMT += write32Bits(fmt_.SampleRate, little_endian);
  FMT += write32Bits(fmt_.ByteRate, little_endian);
  FMT += write16Bits(fmt_.BlockAlign, little_endian);
  FMT += write16Bits(fmt_.BitsPerSample, little_endian);

  return FMT;
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

// read 32 bits from the given string iterator
// iterator will be moved 32 bits
uint32_t read_32bits(string::iterator &it, Endian endian){
  // read the data in the iterator for 4 bytes (uint8_t)
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
  // read the data in the iterator for 4 bytes (uint8_t)
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

string write16Bits(uint16_t in, Endian endian){
  uint8_t bytes[2];
  string bitString16;

  if(endian == big_endian){
    bytes[0] = (in >> 8) & 0xFF;
    bytes[1] = in & 0xFF;
  }
  else{
    bytes[1] = (in >> 8) & 0xFF;
    bytes[0] = in & 0xFF;
  }

  for(int i = 0; i < 2; ++i){
    bitString16 += bytes[i];
  }
  return bitString16;
}

string write32Bits(uint32_t in, Endian endian){
  uint8_t bytes[4];
  string bitString32;

  if(endian == big_endian)
  {
    bytes[0] = (in >> 24) & 0xFF;
    bytes[1] = (in >> 16) & 0xFF;
    bytes[2] = (in >> 8) & 0xFF;
    bytes[3] = in & 0xFF;
  }
  else
  {
    bytes[3] = (in >> 24) & 0xFF;
    bytes[2] = (in >> 16) & 0xFF;
    bytes[1] = (in >> 8) & 0xFF;
    bytes[0] = in & 0xFF;
  }
  for(int i = 0; i < 4; ++i){
    bitString32 += bytes[i];
  }
  return bitString32;
}
