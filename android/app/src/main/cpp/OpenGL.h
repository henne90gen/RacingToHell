#pragma once

#include "android_RacingToHell.h"

#define BUFFER_OFFSET(i) ((void*)(i))

// position X, Y, texture S, T
static const float rect[] = {-1.0f, -1.0f, 0.0f, 1.0f,
                             -1.0f, 1.0f, 0.0f, 0.0f,
                             1.0f, -1.0f, 1.0f, 1.0f,
                             1.0f, 1.0f, 1.0f, 0.0f};

static GLuint texture;
static GLuint buffer;
static GLuint program;

//static GLuint triangleProgram;

static GLint a_position_location;
static GLint a_texture_coordinates_location;
static GLint u_texture_unit_location;

void setupOpenGL(VideoBuffer *videoBuffer);

GLuint createVBO(const GLsizeiptr size, const GLvoid *data, const GLenum usage);

GLuint buildProgramFromAssets(const char *vertex_shader_path, const char *fragment_shader_path);

GLuint buildProgram();

GLuint buildTriangleProgram();

GLuint buildProgram(const GLchar *vertex_shader_source, const GLint vertex_shader_source_length,
                    const GLchar *fragment_shader_source,
                    const GLint fragment_shader_source_length);

GLuint linkProgram(const GLuint vertex_shader, const GLuint fragment_shader);

GLuint compileShader(const GLenum type, const GLchar *source, const GLint length);

void
setTexturePixels(GLuint texture_object_id, VideoBuffer *videoBuffer);

GLuint loadTexture(
        const GLsizei width, const GLsizei height,
        const GLenum type, const GLvoid *pixels);

void swapBuffers(VideoBuffer *videoBuffer);
