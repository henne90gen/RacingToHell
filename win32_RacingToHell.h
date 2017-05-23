struct OffscreenBuffer
{
    BITMAPINFO info;

    unsigned width, height, bytesPerPixel;
    void *content;
};

struct File
{
    void *content;
    
    size_t size;
};