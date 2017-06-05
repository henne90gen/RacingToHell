#pragma once

#include <time.h>
#include <stdlib.h>
#include <X11/cursorfont.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <cstdio>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <alsa/asoundlib.h>

#include "platform.h"
#include "RacingToHell.h"

#define BUFFER_OFFSET(i) ((void*)(i))

// position X, Y, texture S, T
static const float rect[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };

static GLuint texture;
static GLuint buffer;
static GLuint program;

static GLint a_position_location;
static GLint a_texture_coordinates_location;
static GLint u_texture_unit_location;

#define KeyW 25
#define KeyA 38
#define KeyS 39
#define KeyD 40
#define KeySpace 65
#define MouseLeft Button1
#define MouseRight Button3
#define MouseMiddle Button2 // FIXME is this really mouse middle?
#define MouseScrollUp Button4
#define MouseScrollDown Button5

struct GraphicsData {
	Display* display;
	Window window;
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	GLXContext glc;
	XWindowAttributes gwa;
	VideoBuffer videoBuffer;
};

struct AudioData {
	snd_pcm_t *pcm_handle;
	snd_pcm_stream_t stream;
	snd_pcm_hw_params_t *hwparams;
};

long int EVENT_MASK = KeyPressMask | KeyReleaseMask | ButtonPressMask
		| ButtonReleaseMask | PointerMotionMask;
static bool isRunning;

GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

static GraphicsData graphics;
static AudioData audio;
