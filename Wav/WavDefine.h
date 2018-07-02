/*
 *  Definition of wav headers
 *  and enumeration for endians
 *
 *
 */

#include <cstdint>

// RIFF header should begin with this value
#define RIFF_VALUE 0x52494646
// the wanted value in the RIFF format slot
#define WAV_VALUE 0x57415645
// the Wav data portion should begin with this value
#define DATA_VALUE 0x64617461
// FMT headers first four bytes
#define FMT_VALUE 0x666d7420


enum Endian{
  little_endian, big_endian
};

// the wav file begins with a RIFF header
struct RIFF{
  int32_t ChunkID;   //big endian
  int32_t ChunkSize; //little endian
  int32_t Format;    //big endian
};

// the wav file has information about the sound data in an FMT header
struct FMT{
  int32_t SubChunk1ID;   //big endian
  int32_t SubChunk1Size; //little endian
  int16_t AudioFormat;   //little endian
  int16_t NumChannels;   //little endian
  int32_t SampleRate;    //little endian
  int32_t ByteRate;      //little endian
  int16_t BlockAlign;    //little endian
  int16_t BitsPerSample; //little endian
};
