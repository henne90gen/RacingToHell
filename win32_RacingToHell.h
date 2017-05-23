struct OffscreenBuffer
{
    BITMAPINFO info;

    unsigned width, height, bytesPerPixel;
    void *content;
};

