#pragma once
#include "RacingToHell.h"

struct File {

	void* content;
	size_t size;
};

File readFile(char* fileName);
