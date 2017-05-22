#pragma once
#include <iostream>

struct videoBuffer
{
	unsigned width, height, bytesPerPixel;
	
	void *content;
};

void updateAndRender(videoBuffer *buffer);