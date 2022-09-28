#include "Sound.h"

#include "RacingToHell.h"
#include "Resources.h"

namespace Sound {
struct RiffIterator {
    uint8_t *at;
    uint8_t *stop;
};

RiffIterator parseChunkAt(void *at, void *stop) {
    RiffIterator iter;

    iter.at = (uint8_t *)at;
    iter.stop = (uint8_t *)stop;

    return iter;
}

RiffIterator nextChunk(RiffIterator iter) {
    WaveChunk *chunk = (WaveChunk *)iter.at;
    uint32_t size = (chunk->size + 1) & ~1;
    iter.at += sizeof(WaveChunk) + size;

    return iter;
}

bool isValid(RiffIterator iter) { return (iter.at < iter.stop); }

void *getChunkData(RiffIterator iter) { return (iter.at + sizeof(WaveChunk)); }

uint32_t getType(RiffIterator iter) {
    WaveChunk *chunk = (WaveChunk *)iter.at;

    return chunk->id;
}

uint32_t getChunkDataSize(RiffIterator iter) {
    auto chunk = (WaveChunk *)iter.at;
    return chunk->size;
}

LoadedSound loadWAV(GameMemory *memory, const std::string &path) {
    LoadedSound result = {};

    auto resource_opt = get_resource(path);
    if (!resource_opt.has_value()) {
        memory->abort(path + " konnte nicht geladen werden.");
    }

    auto content = resource_opt.value()->get_content(memory);

    auto header = (WaveHeader *)(content.data());
    uint32_t sampleDataSize;
    int16_t *sampleData;

    if (header->riffId != WAVE_CHUNKID_RIFF || header->waveId != WAVE_CHUNKID_WAVE) {
        memory->abort("Invalid WAV header");
    }

    for (RiffIterator iter = parseChunkAt(header + 1, (uint8_t *)(header + 1) + header->chunkSize - 4); isValid(iter);
         iter = nextChunk(iter)) {
        switch (getType(iter)) {
        case (WAVE_CHUNKID_FMT): {
            auto format = (WaveFormat *)getChunkData(iter);
            result.channelCount = format->numChannels;
        } break;

        case (WAVE_CHUNKID_DATA): {
            sampleData = (int16_t *)getChunkData(iter);
            sampleDataSize = getChunkDataSize(iter);
        } break;
        }
    }

    result.sampleCount = sampleDataSize / (result.channelCount * sizeof(int16_t));

    if (result.channelCount == 1) {
        result.samples[0] = (int16_t *)reservePermanentMemory(memory, result.sampleCount * sizeof(int16_t));

        for (uint32_t sampleIndex = 0; sampleIndex < result.sampleCount; ++sampleIndex) {
            result.samples[0][sampleIndex] = sampleData[sampleIndex];
        }
    } else if (result.channelCount == 2) {
        result.samples[0] = (int16_t *)reservePermanentMemory(memory, result.sampleCount * sizeof(int16_t));
        result.samples[1] = (int16_t *)reservePermanentMemory(memory, result.sampleCount * sizeof(int16_t));

        for (uint32_t sampleIndex = 0; sampleIndex < result.sampleCount; ++sampleIndex) {

            int16_t source = sampleData[2 * sampleIndex];
            result.samples[0][sampleIndex] = source;

            source = sampleData[2 * sampleIndex + 1];
            result.samples[1][sampleIndex] = source;
        }
    } else {
        memory->abort("More than two channels aren't supported.");
    }

    return result;
}

void output(GameState *state, Sound::LoadedSound *loadedSound, float volumeLeft, float volumeRight, Mode mode) {
    if (state->lastPlayingSound + 1 >= (float)sizeof(state->playingSounds) / sizeof(state->playingSounds[0])) {
        return;
    }

    ++state->lastPlayingSound;

    state->playingSounds[state->lastPlayingSound].volume[0] = volumeLeft;
    state->playingSounds[state->lastPlayingSound].volume[1] = volumeLeft;
    state->playingSounds[state->lastPlayingSound].loadedSound = *loadedSound;
    state->playingSounds[state->lastPlayingSound].samplesPlayed = 0;
    state->playingSounds[state->lastPlayingSound].mode = mode;
}
} // END namespace Sound

extern "C" GET_SOUND_SAMPLES(getSoundSamples) {
    GameState *gameState = getGameState(memory);

    float *realChannel0 = (float *)reserveTemporaryMemory(memory, sizeof(float) * soundBuffer->sampleCount);
    float *realChannel1 = (float *)reserveTemporaryMemory(memory, sizeof(float) * soundBuffer->sampleCount);

    {
        float *dest0 = realChannel0;
        float *dest1 = realChannel1;

        for (int sampleIndex = 0; sampleIndex < soundBuffer->sampleCount; ++sampleIndex) {
            *dest0++ = 0.0f;
            *dest1++ = 0.0f;
        }
    }

    for (int soundIndex = 0; soundIndex <= gameState->lastPlayingSound; ++soundIndex) {
        Sound::PlayingSound *currentSound = gameState->playingSounds + soundIndex;

        float *dest0 = realChannel0;
        float *dest1 = realChannel1;

        float volume0 = currentSound->volume[0];
        float volume1 = currentSound->volume[1];

        int32_t samplesToMix = soundBuffer->sampleCount;
        int32_t samplesRemainingInInput = currentSound->loadedSound.sampleCount - currentSound->samplesPlayed;

        if (currentSound->mode == Sound::PLAY_ONCE && samplesToMix > samplesRemainingInInput) {
            samplesToMix = samplesRemainingInInput;
        }

        if (currentSound->loadedSound.channelCount == 1) {
            for (unsigned sampleIndex = currentSound->samplesPlayed;
                 sampleIndex < currentSound->samplesPlayed + samplesToMix; ++sampleIndex) {
                int index = sampleIndex % currentSound->loadedSound.sampleCount;
                float sampleValue = currentSound->loadedSound.samples[0][index];

                *dest0++ += volume0 * sampleValue;
                *dest1++ += volume1 * sampleValue;
            }
        } else {
            for (unsigned sampleIndex = currentSound->samplesPlayed;
                 sampleIndex < currentSound->samplesPlayed + samplesToMix; ++sampleIndex) {
                int index = sampleIndex % currentSound->loadedSound.sampleCount;
                float sampleValue0 = currentSound->loadedSound.samples[0][index];
                float sampleValue1 = currentSound->loadedSound.samples[1][index];

                *dest0++ += volume0 * sampleValue0;
                *dest1++ += volume1 * sampleValue1;
            }
        }

        currentSound->samplesPlayed += samplesToMix;

        if (currentSound->mode == Sound::PLAY_ONCE &&
            currentSound->samplesPlayed >= currentSound->loadedSound.sampleCount) {
            *currentSound = gameState->playingSounds[gameState->lastPlayingSound];
            --gameState->lastPlayingSound;
            --soundIndex;
        }
    }

    {
        float *source0 = realChannel0;
        float *source1 = realChannel1;

        int16_t *sampleOut = soundBuffer->samples;
        for (int sampleIndex = 0; sampleIndex < soundBuffer->sampleCount; ++sampleIndex) {
            *sampleOut++ = (int16_t)(*source0++ + 0.5f);
            *sampleOut++ = (int16_t)(*source1++ + 0.5f);
        }
    }

    freeTemporaryMemory(memory);
}
