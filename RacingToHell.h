#pragma once
#include <iostream>

struct VideoBuffer
{
	unsigned width, height, bytesPerPixel;
	
	void *content;
};

void updateAndRender(VideoBuffer *buffer);