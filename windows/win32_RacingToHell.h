struct OffscreenBuffer
{
    BITMAPINFO info;

    unsigned width, height, bytesPerPixel;
    void *content;
};

struct SoundOutput
{
    int samplesPerSecond = 48000;
    uint32_t runningSampleIndex = 0;
    int bytesPerSample = sizeof(uint16_t) * 2;
    int secondaryBufferSize = samplesPerSecond * bytesPerSample;
    int safetyBytes;
};

struct ExeFilename
{
    char filename[MAX_PATH];
    char *onePastLastExeFilenameSash;
};

struct GameCode
{
    HMODULE gameCodeDLL;
    bool isValid = false;

    FILETIME lastWriteTime;

    //_GetSoundSamples_ *getSoundSamples;
    update_and_render *updateAndRender;
};