#pragma once
#include <iostream>
#include "platform.h"

struct VideoBuffer
{
	unsigned width, height, bytesPerPixel;
	
	void *content;
};

void updateAndRender(VideoBuffer *buffer);
