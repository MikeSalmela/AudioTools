/*
 *  Definition of wav headers
 *  and enumeration for endians
 *
 *
 */

#include <cstdint>

#define RIFF_VALUE 0x52494646
#define WAV_VALUE 0x57415645

enum Endian{
  little_endian, big_endian
};

// the wav file begins with a RIFF header
struct RIFF{
  uint32_t ChunkID;
  uint32_t ChunkSize;
  uint32_t Format;
};

// the wav file has information about the sound data in an FMT header
struct FMT{
  uint32_t SubChunk1ID;
  uint32_t SubChunk1Size;
  uint16_t AudioFormat;
  uint16_t NumChannels;
  uint32_t SampleRate;
  uint32_t ByteRate;
  uint16_t BlockAlign;
  uint16_t BitsPerSample;
};
