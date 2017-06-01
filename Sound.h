#pragma once

#include "platform.h"

namespace Sound
{
    struct LoadedSound 
    {
        uint32_t sampleCount;
        uint32_t channelCount;
        int16_t *samples[2];
    };

#pragma pack(push)
    struct WaveHeader
    {
        uint32_t riffId;
        uint32_t chunkSize;
        uint32_t waveId;
    };

    struct WaveChunk
    {
        uint32_t id;
        uint32_t size;
    };

    struct WaveFormat
    {
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;

        uint16_t validBitsPerSample;
        uint32_t channelMask;
        uint8_t subFormat;
    };
#pragma pack(pop)

#define RIFF_CODE(a, b, c, d) (((uint32_t)(a) << 0) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))

    enum
    {
        WAVE_CHUNKID_FMT = RIFF_CODE('f', 'm', 't', ' '),
        WAVE_CHUNKID_DATA = RIFF_CODE('d', 'a', 't', 'a'),
        WAVE_CHUNKID_RIFF = RIFF_CODE('R', 'I', 'F', 'F'),
        WAVE_CHUNKID_WAVE = RIFF_CODE('W', 'A', 'V', 'E')
    };

    LoadedSound loadWAV(GameMemory *memory, std::string path);
}