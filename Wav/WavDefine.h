/*
 *  Definition of wav headers
 *  and enumeration for endians
 *
 *
 */

#include <cstdint>

enum Endian{
  little_endian, big_endian
};

struct RIFF{
  uint32_t ChunkID;
  uint32_t ChunkSize;
  uint32_t Format;
};

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
