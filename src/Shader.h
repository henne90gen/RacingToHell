#pragma once

#include <glad/glad.h>

#include "Platform.h"
#include "Resources.h"

GLuint buildProgram(GameMemory *memory, Resource *vertex_shader, Resource *fragment_shader);
