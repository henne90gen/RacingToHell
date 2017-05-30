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