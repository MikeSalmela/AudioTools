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
  uint32_t ChunkID;   //big endian
  uint32_t ChunkSize; //little endian
  uint32_t Format;    //big endian
};

// the wav file has information about the sound data in an FMT header
struct FMT{
  uint32_t SubChunk1ID;   //big endian
  uint32_t SubChunk1Size; //little endian
  uint16_t AudioFormat;   //little endian
  uint16_t NumChannels;   //little endian
  uint32_t SampleRate;    //little endian
  uint32_t ByteRate;      //little endian
  uint16_t BlockAlign;    //little endian
  uint16_t BitsPerSample; //little endian
};
