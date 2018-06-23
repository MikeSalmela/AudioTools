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
// Return the RAW data portion of the wav file
vector<uint8_t> const Wav::get_RAW_data(){
  return data_;
}
vector<int16_t> const Wav::get_16bit_data(){
  return data16bit_;
}



// Returns the wav file as a string
string Wav::getWAV(){
  string WAVFile;
  WAVFile += getRIFF();
  WAVFile += getFMT();
  WAVFile += write_N_bits(SubChunk2ID_, big_endian, 4);
  WAVFile += write_N_bits(SubChunk2Size_, little_endian, 4);
  if((fmt_.BitsPerSample == 8)){
    for(uint8_t c : data_){
      WAVFile += c;
    }
  }
  else if(fmt_.BitsPerSample == 16){
    for(int16_t c : data16bit_){
      WAVFile += write_N_bits(c, big_endian, 2);
    }
  }
  else{
    throw (MyException("samplerate not implemented"));
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
  //fmt_.SubChunk1Size = 16;  // Skip extra chunks
  fmt_.AudioFormat = read_16bits(it, little_endian);
  fmt_.NumChannels = read_16bits(it, little_endian);
  fmt_.SampleRate = read_32bits(it, little_endian);
  fmt_.ByteRate = read_32bits(it, little_endian);
  fmt_.BlockAlign = read_16bits(it, little_endian);
  fmt_.BitsPerSample = read_16bits(it, little_endian);


  if(fmt_.NumChannels != 1){
    throw(MyException("Stereo sound not supported"));
  }

  if(fmt_.SubChunk1ID != FMT_VALUE){
    throw( MyException("not FMT"));
  }
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
    // Try agian
    if(!fillSubChunk2(it)){
      throw MyException("Data ID not found");
    }
  }

}

// Reads and saves the SubChunk2 data. Returns true when the ID is correct
bool Wav::fillSubChunk2(string::iterator &it){
  SubChunk2ID_ = read_32bits(it, big_endian);

  // check ID
  if(SubChunk2ID_ != DATA_VALUE) return false;
  SubChunk2Size_ = read_32bits(it, little_endian);
  if(fmt_.BitsPerSample == 8){
    read_8bit_sample(it);
  }
  else{
    read_16bit_sample(it);
  }
  return true;


}

void Wav::read_8bit_sample(string::iterator &it)
{
  for(int i = 0; i < SubChunk2Size_; ++i)
  {
    data_.push_back(*it);
    ++it;
  }
}

void Wav::read_16bit_sample(string::iterator &it)
{
  for(int i = 0; i < SubChunk2Size_/2; ++i){
    data16bit_.push_back(read_16bits(it, big_endian));
  }
}





// Create a string from the Wav RIFF header
string Wav::getRIFF(){
  string RIFF;
  RIFF += write_N_bits(riff_.ChunkID, big_endian, 4);
  RIFF += write_N_bits(riff_.ChunkSize, little_endian, 4);
  RIFF += write_N_bits(riff_.Format, big_endian, 4);

  return RIFF;
}

// Create a string from the Wav FMT header
string Wav::getFMT(){
  string FMT;
  FMT += write_N_bits(fmt_.SubChunk1ID, big_endian, 4);
  FMT += write_N_bits(fmt_.SubChunk1Size, little_endian, 4);
  FMT += write_N_bits(fmt_.AudioFormat, little_endian, 2);
  FMT += write_N_bits(fmt_.NumChannels, little_endian, 2);
  FMT += write_N_bits(fmt_.SampleRate, little_endian, 4);
  FMT += write_N_bits(fmt_.ByteRate, little_endian, 4);
  FMT += write_N_bits(fmt_.BlockAlign, little_endian, 2);
  FMT += write_N_bits(fmt_.BitsPerSample, little_endian, 2);

  return FMT;
}

// Read the whole file into a string
string file_to_string(const string &filename){
  ifstream inputfile(filename);
  if(!inputfile){
    inputfile.close();
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
int16_t read_16bits(string::iterator &it, Endian endian){
  // read the data in the iterator for 2 bytes
  uint8_t read_data[2];
  for(int i = 0; i < 2; ++i){
    read_data[i] = *it;
    ++it;
  }
  int16_t data = 0;
  // Shift the bits to form a 16 bit variable
  if(endian == little_endian){
    data = ((read_data[0]) | read_data[1] << 8);
  }
  else{
    data = (read_data[0] << 8 | read_data[1]);
  }
  return data;
}

// Returns a string with the in data written accordingly to the Endian
string write_N_bits(int32_t in, Endian endian, int n){
  uint8_t bytes[n];
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
