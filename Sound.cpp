#include "Sound.h"

namespace Sound
{
    struct RiffIterator
    {
        uint8_t *at;
        uint8_t *stop;
    };

    RiffIterator parseChunkAt(void *at, void *stop)
    {
        RiffIterator iter;

        iter.at = (uint8_t *)at;
        iter.stop = (uint8_t *)stop;

        return iter;
    }

    RiffIterator nextChunk(RiffIterator iter)
    {
        WaveChunk *chunk = (WaveChunk *)iter.at;
        uint32_t size = (chunk->size + 1) & ~1;
        iter.at += sizeof(WaveChunk) + size;

        return iter;
    }

    bool isValid(RiffIterator iter)
    {
        return (iter.at < iter.stop);
    }

    void *getChunkData(RiffIterator iter)
    {
        return (iter.at + sizeof(WaveChunk));
    }

    uint32_t getType(RiffIterator iter)
    {
        WaveChunk *chunk = (WaveChunk *)iter.at;

        return chunk->id;
    }

    uint32_t getChunkDataSize(RiffIterator iter)
    {
        WaveChunk *chunk = (WaveChunk *)iter.at;

        return chunk->size;
    }

    struct LoadedSound
    {
        uint32_t sampleCount;
        uint32_t channelCount;
        uint16_t *samples[2];
    };

    void loadWAV(GameMemory *memory, std::string path)
    {
        LoadedSound result;

        File file = readFile(path);

        if (file.content == NULL)
        {
            abort(path + " konnte nicht geladen werden.");
        }

        WaveHeader *header = (WaveHeader *)(file.content);
        uint32_t sampleDataSize;
        uint16_t *sampleData;

        if (header->riffId != WAVE_CHUNKID_RIFF || header->waveId != WAVE_CHUNKID_WAVE)
        {
            abort("Ungueltiger Header");
        }

        for (RiffIterator iter = parseChunkAt(header + 1, (uint8_t *)(header + 1) + header->chunkSize - 4); 
            isValid(iter); 
            iter = nextChunk(iter))
        {
            switch (getType(iter))
            {
                case (WAVE_CHUNKID_FMT):
                {
                    WaveFormat *format = (WaveFormat *)getChunkData(iter);
                    result.channelCount = format->numChannels;
                } break;

                case (WAVE_CHUNKID_DATA):
                {
                    sampleData = (uint16_t*)getChunkData(iter);
                    sampleDataSize = getChunkDataSize(iter);
                } break;
            }
        }

        result.sampleCount = sampleDataSize / (result.channelCount * sizeof(uint16_t));
        
        if (result.channelCount == 1)
        {
            result.samples[0] = sampleData;
            result.samples[1] = 0;
        }
        else if (result.channelCount == 2)
        {
            result.samples[0] = sampleData;
            result.samples[1] = sampleData + result.sampleCount;

            for (uint32_t sampleIndex = 0; sampleIndex < result.sampleCount; ++sampleIndex)
            {
                uint16_t source = sampleData[2 * sampleIndex];
                sampleData[2 * sampleIndex] = sampleData[sampleIndex];
                sampleData[sampleIndex] = source;
            }
        }
        else
        {
            abort("Mehr als 2 Channeles werden nicht unterstuezt.");
        }

        freeFile(&file); //?
    }
}