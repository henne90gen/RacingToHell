#pragma once

#include <time.h>
#include <stdlib.h>
#include <cstdio>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>

#include <X11/cursorfont.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/xf86vmode.h>

#include <alsa/asoundlib.h>
#include <dlfcn.h>
#include <sys/stat.h>

#include "Platform.h"
#include "RacingToHell.h"

// position X, Y, texture S, T
static const float rect[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };

// Giving keys more descriptive names
#define KeyEscape 9
#define KeyEnter 36
#define KeyW	 25
#define KeyA	 38
#define KeyS	 39
#define KeyD	 40
#define KeyF1	 67
#define KeyF11	 95

// Giving mouse buttons more descriptive names
#define MouseLeft       Button1
#define MouseMiddle     Button2
#define MouseRight      Button3
#define MouseScrollUp   Button4
#define MouseScrollDown Button5

struct GraphicsData {
	Display* display;
	Window window;
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	GLXContext glc;
	XWindowAttributes gwa;

	GLuint gl_texture;
	GLuint gl_buffer;
	GLuint gl_program;

	GLint gl_position_location;
	GLint gl_texture_coordinates_location;
	GLint gl_texture_unit_location;

	VideoBuffer videoBuffer;
};

struct AudioData {
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *hw_params;
	int16_t *buffer;

	// taken from xcb_handmade
	int samples_per_second;
	uint32_t running_sample_index;
	int bytes_per_sample;
	uint32_t buffer_size_in_samples;
	uint32_t buffer_size_in_bytes;
	uint32_t safety_samples;
	uint32_t channels;
	uint64_t period_size;
};

struct GameCode {
	void *libraryHandle;
	time_t libraryMTime;

	update_and_render* updateAndRender;
	get_sound_samples* getSoundSamples;
};

long int EVENT_MASK = KeyPressMask | KeyReleaseMask | ButtonPressMask
		| ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;

static bool isRunning;
static GameCode gameCode;

