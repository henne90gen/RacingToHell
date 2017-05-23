#pragma once
#include <iostream>
#include <stdint.h>
#include "platform.h"

struct VideoBuffer
{
	unsigned width, height, bytesPerPixel;
	
	void *content;
};

struct Input
{
    bool upKey, downKey, leftKey, rightKey;
    bool pauseKey;
    bool shootKey;
};

void updateAndRender(VideoBuffer *buffer, Input *input);
