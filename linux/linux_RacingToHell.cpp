#include "linux_RacingToHell.h"

/**
 * Initializes the audio context
 */
AudioData initAudioData() {
	rth_log("Loading audio context.");

	AudioData audio = { };

	audio.samples_per_second = 48000;
	audio.channels = 2;
	audio.bytes_per_sample = sizeof(int16_t) * audio.channels;
	audio.buffer_size_in_samples = audio.samples_per_second;
	audio.buffer_size_in_bytes = audio.buffer_size_in_samples
			* audio.bytes_per_sample;
	audio.safety_samples =
			(uint32_t) (((float) audio.samples_per_second / 60.0f) / 3.0f);

	char *pcm_name;
	pcm_name = strdup("default");
	int error;
	if ((error = snd_pcm_open(&audio.pcm_handle, pcm_name,
			SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		std::string errorMsg = "Error opening PCM device "
				+ std::string(pcm_name) + " "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	snd_pcm_hw_params_alloca(&audio.hw_params);

	if ((error = snd_pcm_hw_params_any(audio.pcm_handle, audio.hw_params))
			< 0) {
		std::string errorMsg = "Cannot configure PCM device. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	if ((error = snd_pcm_hw_params_set_access(audio.pcm_handle, audio.hw_params,
			SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		std::string errorMsg = "Cannot set access type. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	if ((error = snd_pcm_hw_params_set_format(audio.pcm_handle, audio.hw_params,
			SND_PCM_FORMAT_S16_LE)) < 0) {
		std::string errorMsg = "Cannot set sample format. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	if ((error = snd_pcm_hw_params_set_channels(audio.pcm_handle,
			audio.hw_params, audio.channels)) < 0) {
		std::string errorMsg = "Cannot set channel count. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	if ((error = snd_pcm_hw_params_set_rate(audio.pcm_handle, audio.hw_params,
			audio.samples_per_second, 0)) < 0) {
		std::string errorMsg = "Cannot set sample rate. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	if ((error = snd_pcm_hw_params_set_buffer_size(audio.pcm_handle,
			audio.hw_params, audio.buffer_size_in_samples)) < 0) {
		std::string errorMsg = "Cannot set buffer size. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	if ((error = snd_pcm_hw_params(audio.pcm_handle, audio.hw_params)) < 0) {
		std::string errorMsg = "Cannot set hardware parameters. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	rth_log("PCM name: " + std::string(snd_pcm_name(audio.pcm_handle)));
	std::string pcmState = std::string(
			snd_pcm_state_name(snd_pcm_state(audio.pcm_handle)));
	rth_log("PCM state: " + pcmState);

	snd_pcm_hw_params_get_channels(audio.hw_params, &audio.channels);
	rth_log("Channels: " + std::to_string(audio.channels));

	rth_log("Rate: " + std::to_string(audio.samples_per_second));

	if ((error = snd_pcm_hw_params_get_period_size(audio.hw_params,
			&audio.period_size, 0)) < 0) {
		std::string errorMsg = "Couldn't get period size. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	rth_log("Period size: " + std::to_string(audio.period_size));

	int32_t maxPossibleOverrun = 2 * 8 * sizeof(int16_t);
	audio.buffer = (int16_t *) malloc(
			audio.buffer_size_in_bytes + maxPossibleOverrun);

	rth_log("Successfully loaded audio context.");
	return audio;
}

Render::Texture loadIcon(std::string iconName, void** content) {
	File file = readFile(iconName);
	if (((char*) file.content)[0] != 'B' || (file.content)[1] != 'M') {
		abort(file.name + " is not a bitmap file.");
	}
	int fileHeaderSize = 14;
	BitmapHeader header = *((BitmapHeader*) (file.content + fileHeaderSize));

	if (header.bitsPerPixel != 32) {
		abort("Image must have 32-bit of color depth.");
	}

	Render::Texture icon = { };
	icon.width = header.width;
	icon.height = header.height;
	icon.bytesPerPixel = header.bitsPerPixel / 8;
	*content = malloc(icon.width * icon.height * icon.bytesPerPixel);

	void* src = file.content + header.size + fileHeaderSize;
	void* dest = *content;
	for (unsigned y = 0; y < icon.height; y++) {
		for (unsigned x = 0; x < icon.width; x++) {
			int srcIndex = (header.height - y - 1) * header.width + x;
			int destIndex = y * icon.width + x;
			uint32_t color = ((uint32_t*) (src))[srcIndex];
			uint8_t red = (color & 0xff000000) >> 24;
			uint8_t green = (color & 0x00ff0000) >> 16;
			uint8_t blue = (color & 0x0000ff00) >> 8;
			uint8_t alpha = color & 0x000000ff;
			color = (alpha << 24) + (red << 16) + (green << 8) + blue;
			((uint32_t*) (dest))[destIndex] = color;
		}
	}
	free(file.content);
	return icon;
}

/**
 * Opens a window and initializes the graphics context
 */
GraphicsData initGraphicsData() {
	GraphicsData graphics = { };
	graphics.display = XOpenDisplay(NULL);
	if (!graphics.display) {
		abort("Cannot open display.");
	};

	Window root = DefaultRootWindow(graphics.display);
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	graphics.vi = glXChooseVisual(graphics.display, 0, att);
	if (!graphics.vi) {
		abort("No appropriate visual found.");
	}
	graphics.cmap = XCreateColormap(graphics.display, root, graphics.vi->visual,
	AllocNone);

	graphics.swa.colormap = graphics.cmap;
	graphics.swa.event_mask = EVENT_MASK;
	graphics.window = XCreateWindow(graphics.display, root, 0, 0,
	DEFAULT_WINDOW_WIDTH,
	DEFAULT_WINDOW_HEIGHT, 0, graphics.vi->depth, InputOutput,
			graphics.vi->visual,
			CWColormap | CWEventMask, &graphics.swa);

	graphics.glc = glXCreateContext(graphics.display, graphics.vi, NULL,
	GL_TRUE);
	glXMakeCurrent(graphics.display, graphics.window, graphics.glc);
	if (glewInit() != GLEW_OK) {
		abort("Failed to initialize GLEW.");
	}

	XGetWindowAttributes(graphics.display, graphics.window, &graphics.gwa);
	glViewport(0, 0, graphics.gwa.width, graphics.gwa.height);

	// set window title
	XStoreName(graphics.display, graphics.window, WINDOW_TITLE);

	// set icon name
	XSetIconName(graphics.display, graphics.window, WINDOW_TITLE);

	// set class name
	XClassHint* classHint = XAllocClassHint();
	char name[] = WINDOW_TITLE;
	classHint->res_name = name;
	classHint->res_class = name;
	XSetClassHint(graphics.display, graphics.window, classHint);

	// set icon
	Atom iconAtom = XInternAtom(graphics.display, "_NET_WM_ICON", 0);
	void *iconContent = NULL;
	Render::Texture icon = loadIcon("./res/icon.bmp", &iconContent);
	int propsize = 2 + (icon.width * icon.height);
	long *propdata = (long*) malloc(propsize * sizeof(long));

	propdata[0] = icon.width;
	propdata[1] = icon.height;
	uint32_t *src;
	long *dst = &propdata[2];
	for (unsigned y = 0; y < icon.height; ++y) {
		src = (uint32_t*) ((uint8_t*) iconContent + y * (icon.width * 4));
		for (unsigned x = 0; x < icon.width; ++x) {
			uint32_t color = 0;
			uint8_t alpha = (color & 0xff000000) >> 24;
			uint8_t blue = (color & 0x00ff0000) >> 16;
			uint8_t green = (color & 0x0000ff00) >> 8;
			uint8_t red = (color & 0x000000ff);
			color = (alpha << 24) + (red << 16) + (green << 8) + blue;
			*dst++ = *src++;
		}
	}
	XChangeProperty(graphics.display, graphics.window, iconAtom,
	XA_CARDINAL, 32, PropModeReplace, (unsigned char *) propdata, propsize);
	free(iconContent);

	// subscribe to events
	XSelectInput(graphics.display, graphics.window, EVENT_MASK);

	// change cursor
	XDefineCursor(graphics.display, graphics.window,
			XCreateFontCursor(graphics.display, XC_arrow));

	Atom wmDeleteWindow = XInternAtom(graphics.display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(graphics.display, graphics.window, &wmDeleteWindow, 1);

	XMapWindow(graphics.display, graphics.window);
	XFlush(graphics.display);

	return graphics;
}

void toggleFullscreen(GraphicsData *graphics) {
	static bool isFullscreen = false;
	isFullscreen = !isFullscreen;

	Atom wmState = XInternAtom(graphics->display, "_NET_WM_STATE", False);
	Atom fullscreen = XInternAtom(graphics->display, "_NET_WM_STATE_FULLSCREEN",
	False);

	XEvent xev;
	xev.xclient.type = ClientMessage;
	xev.xclient.serial = 0;
	xev.xclient.send_event = True;
	xev.xclient.window = graphics->window;
	xev.xclient.message_type = wmState;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = (isFullscreen ? 1 : 0);
	xev.xclient.data.l[1] = fullscreen;
	xev.xclient.data.l[2] = 0;

	XSendEvent(graphics->display, DefaultRootWindow(graphics->display), False,
	SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}

/**
 * Exits the game on error
 */
ABORT(abort) {
	fprintf(stderr, "%s", (message + "\n").c_str());
	exit(1);
}

/**
 * Uses 'printf' to log messages to the console
 */
LOG(rth_log) {
	printf("%s", (message + "\n").c_str());
}

/**
 * Returns the time in nanoseconds that has passed since the program start
 */
QUERY_TIME(queryTime) {
	timespec time = {};
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
	return time.tv_nsec + 1000000000 * time.tv_sec;
}

/**
 * Reads an entire file into memory
 * This might be a very expensive operation, if the file is very large
 */
READ_FILE(readFile) {
	rth_log("Reading " + fileName);

	FILE *fileHandle = fopen(fileName.c_str(), "rb");
	if (!fileHandle) {
		abort("Could not open file " + fileName);
	}
	fseek(fileHandle, 0, SEEK_END);
	long int fileSize = ftell(fileHandle);
	rewind(fileHandle);

	char* content = (char*) malloc(fileSize + 1);
	fread(content, fileSize, 1, fileHandle);
	fclose(fileHandle);

	content[fileSize] = 0; // 0-terminated

	File file = { };
	file.size = fileSize;
	file.name = fileName;
	file.content = content;

	return file;
}

/**
 * Writes an entire file to disk
 */
WRITE_FILE(writeFile) {
	rth_log("Writing " + fileName);

	FILE *fHandle = fopen(fileName.c_str(), "wb");
	if (fHandle == NULL) {
		rth_log("ERROR - Failed to open file for writing");
		return false;
	}

	if (fwrite((void *) file->content, 1, file->size, fHandle) != file->size) {
		std::string errorMsg = "ERROR - Failed to write "
				+ std::to_string(file->size) + " bytes to file";
		rth_log(errorMsg);
		return false;
	}

	fclose(fHandle);
	fHandle = NULL;
	return true;
}

/**
 * Releases the memory that was allocated for the given file
 */
FREE_FILE(freeFile) {
	if (file->content) {
		free(file->content);
	}

	file->size = 0;
}

/**
 * Ends the game gracefully
 */
EXIT_GAME(exitGame) {
	rth_log("Exiting");
	isRunning = false;
}

/**
 * Changes the OpenGL viewport to the desired aspect ratio within the given window
 */
void resizeViewport(GraphicsData *graphics, GameMemory *memory) {
	int viewWidth = graphics->windowHeight * memory->aspectRatio;
	int viewHeight = graphics->windowHeight;
	int offsetX = 0;
	int offsetY = 0;
	if (viewWidth > graphics->windowWidth) {
		viewWidth = graphics->windowWidth;
		viewHeight = graphics->windowWidth / memory->aspectRatio;
	} else {
		offsetX = (graphics->windowWidth - viewWidth) / 2;
	}
	if (graphics->windowHeight > viewHeight) {
		offsetY = (graphics->windowHeight - viewHeight) / 2;
	}

	glViewport(offsetX, offsetY, viewWidth, viewHeight);

	ViewProperties props = { };
	props.width = viewWidth;
	props.height = viewHeight;
	props.offsetX = offsetX;
	props.offsetY = offsetY;
	graphics->viewProps = props;

	memory->doResize = true;
}

void toggleAspectRatio(GameMemory* memory, GraphicsData* graphics) {
	if (memory->aspectRatio < 1.5) {
		memory->aspectRatio = 16.0 / 9.0;
	} else {
		memory->aspectRatio = 4.0 / 3.0;
	}
	resizeViewport(graphics, memory);
}

/**
 * Uses the native key event to fill the platform independent input struct
 */
void handleKeyEvent(GameMemory *memory, GraphicsData* graphics, Input* input,
		XKeyEvent event) {
	bool keyPressed = event.type == KeyPress;

//	rth_log("Key pressed: " + event.keycode);

	switch (event.keycode) {
	case KeyF1:
		exitGame();
		break;
	case KeyF11:
		if (keyPressed) {
			toggleFullscreen(graphics);
		}
		break;
	case KeyF2:
		if (keyPressed) {
			toggleAspectRatio(memory, graphics);
		}
		break;
	case KeyW:
		input->upKeyPressed = keyPressed;
		break;
	case KeyA:
		input->leftKeyPressed = keyPressed;
		break;
	case KeyS:
		input->downKeyPressed = keyPressed;
		break;
	case KeyD:
		input->rightKeyPressed = keyPressed;
		break;
	case KeyEscape:
		input->escapeKeyPressed = keyPressed;
		break;
	case KeyEnter:
		input->enterKeyPressed = keyPressed;
		break;
	case KeyPlus:
		input->plusKeyPressed = keyPressed;
		break;
	case KeyMinus:
		input->minusKeyPressed = keyPressed;
		break;
	}
}

/**
 * Uses the native mouse event to fill the platform independent input struct
 */
void handleMouseEvent(Input* input, XButtonEvent event,
		bool *wasLeftMousePressed, bool *wasRightMousePressed) {
	bool buttonPressed = event.type == ButtonPress;

	switch (event.button) {
	case MouseLeft:
		input->shootKeyPressed = buttonPressed;
		input->shootKeyClicked = buttonPressed && !*wasLeftMousePressed;
		*wasLeftMousePressed = buttonPressed;
		break;
	case MouseMiddle:
		break;
	case MouseRight:
		input->shootKeyPressed = buttonPressed;
		input->shootKeyClicked = buttonPressed && !*wasRightMousePressed;
		*wasRightMousePressed = buttonPressed;
		break;
	case MouseScrollUp:
		break;
	case MouseScrollDown:
		break;
	}

}

/**
 * Puts the processor to sleep if we were faster than 16.7ms
 */
void correctTiming(GraphicsData *graphics, timespec startTime,
		bool consoleOutput) {
	timespec endTime = { };
	clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
	if (endTime.tv_nsec < startTime.tv_nsec) {
		return;
	}
	long nanoSecondsElapsed = endTime.tv_nsec - startTime.tv_nsec;

	const float targetFrameTime = 1000000000.0f / 60.0f;
	if (nanoSecondsElapsed < targetFrameTime) {
		timespec sleepTime = { };
		sleepTime.tv_sec = (targetFrameTime - nanoSecondsElapsed)
				/ 1000000000.0f;
		sleepTime.tv_nsec = targetFrameTime - nanoSecondsElapsed;
		nanosleep(&sleepTime, NULL);
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
	if (endTime.tv_nsec < startTime.tv_nsec) {
		return;
	}
	nanoSecondsElapsed = endTime.tv_nsec - startTime.tv_nsec;

	std::stringstream ss;
	ss << WINDOW_TITLE << ": "
			<< std::to_string(nanoSecondsElapsed / 1000000.0f);
	ss << "ms, " << std::to_string(1000000000.0f / nanoSecondsElapsed);
	ss << " FPS";
	XStoreName(graphics->display, graphics->window, ss.str().c_str());
	XFlush(graphics->display);

	if (consoleOutput) {
		// FIXME use same string for window title and console
		std::string frameTimeAndFrameRate = "Frametime: "
				+ std::to_string(nanoSecondsElapsed / 1000000.0f)
				+ "ms, Framerate: "
				+ std::to_string(1000000000.0f / nanoSecondsElapsed);
		rth_log(frameTimeAndFrameRate);
	}
}

/**
 * Feeds ALSA the next interval that needs to be played
 */
void swapSoundBuffers(AudioData *audio, GameMemory *memory) {
	// this implementation is from https://github.com/nxsy/xcb_handmade
	snd_pcm_sframes_t delay, avail;
	int error = snd_pcm_avail_delay(audio->pcm_handle, &avail, &delay);
	if (error < 0) {
		// audio will cut out eventually, if we don't recover
		snd_pcm_recover(audio->pcm_handle, error, true);
	}

	int32_t samplesToFill = 0;
	int32_t expectedSamplesPerFrame =
			((float) audio->samples_per_second / 60.0f);
	int32_t samplesInBuffer = audio->buffer_size_in_samples - avail;

	if (samplesInBuffer < 0) {
		abort("SamplesInBuffer below 0: " + samplesInBuffer);
	}

	int32_t sampleAdjustment = (samplesInBuffer - (audio->period_size * 4)) / 2;
	if (audio->buffer_size_in_samples == avail) {
		// NOTE: initial fill on startup and after an underrun
		samplesToFill = (expectedSamplesPerFrame * 2) + audio->safety_samples;
	} else {
		samplesToFill = expectedSamplesPerFrame - sampleAdjustment;
		if (samplesToFill < 0) {
			samplesToFill = 0;
		}
	}

	if (samplesToFill < 0) {
		abort("SamplesToFill below 0: " + samplesToFill);
	}

	SoundBuffer soundBuffer = { };
	soundBuffer.samplesPerSecond = audio->samples_per_second;
	soundBuffer.sampleCount = (((samplesToFill) + (7)) & ~(7));
	soundBuffer.samples = audio->buffer;

	if (soundBuffer.sampleCount < 0) {
		abort("SampleCount below 0: " + soundBuffer.sampleCount);
	}

	gameCode.getSoundSamples(memory, &soundBuffer);

#if SOUND_DEBUG
	// NOTE: "delay" is the delay of the soundcard hardware
	rth_log("Samples in buffer before write: " + std::to_string(audio->buffer_size_in_samples - avail) + " | Delay in samples: " + std::to_string(delay));
#endif

	int32_t writtenSamples = snd_pcm_writei(audio->pcm_handle, audio->buffer,
			soundBuffer.sampleCount);
	if (writtenSamples < 0) {
		writtenSamples = snd_pcm_recover(audio->pcm_handle, writtenSamples,
				true);
	} else if (writtenSamples != soundBuffer.sampleCount) {
		std::string message = "Only wrote " + std::to_string(writtenSamples)
				+ " of " + std::to_string(soundBuffer.sampleCount)
				+ " samples.";
		rth_log(message);
	}

#if SOUND_DEBUG
	snd_pcm_avail_delay(audio->pcm_handle, &avail, &delay);
	rth_log("Samples in buffer after write: " + std::to_string(audio->buffer_size_in_samples - avail + writtenSamples) + " | Delay in samples: " + std::to_string(delay) + " | Written samples: " + std::to_string(writtenSamples));
#endif
}

/**
 * Loads the game code library and fetches the required methods from it
 */
GameCode loadGameCode() {
	rth_log("Loading GameCode.");
	GameCode result = { };

	struct stat statbuf = { };
	uint32_t stat_result = stat("./librth.so", &statbuf);
	if (stat_result != 0) {
		abort("Failed to read modification time of game code library.");
	}
	result.libraryMTime = statbuf.st_mtime;

	result.libraryHandle = dlopen("./librth.so", RTLD_NOW);
	if (result.libraryHandle) {
		result.updateAndRender = (update_and_render *) dlsym(
				result.libraryHandle, "updateAndRender");
		result.getSoundSamples = (get_sound_samples *) dlsym(
				result.libraryHandle, "getSoundSamples");
		if (!result.updateAndRender) {
			abort("Couldn't load 'updateAndRender' game function.");
		}
		if (!result.getSoundSamples) {
			abort("Couldn't load 'getSoundSamples' game function.");
		}
	} else {
		abort("Couldn't load library. " + std::string(dlerror()));
	}
	return result;
}

/**
 * Unloads the game code library
 */
void unloadGameCode(GameCode *code) {
	rth_log("Unloading GameCode.");

	if (code->libraryHandle) {
		dlclose(code->libraryHandle);
		code->libraryHandle = 0;
	}
}

/**
 * Allocates the game memory
 */
GameMemory initGameMemory() {
	GameMemory memory = { };
	memory.doResize = true;
	memory.aspectRatio = 16.0f / 9.0f;

	memory.abort = abort;
	memory.log = rth_log;
	memory.queryTime = queryTime;
	memory.readFile = readFile;
	memory.freeFile = freeFile;
	memory.exitGame = exitGame;

	memory.temporaryMemorySize = 10 * 1024 * 1024;
	memory.permanentMemorySize = 100 * 1024 * 1024;
	memory.temporary = (char*) malloc(memory.temporaryMemorySize);
	memory.permanent = (char*) malloc(memory.permanentMemorySize);

	return memory;
}

/**
 * Cuts off anything that is not inside -range <--> range
 */
float cullToRange(float value, float range) {
	if (value < -range) {
		return -range;
	} else if (value > range) {
		return range;
	}
	return value;
}

Math::Vector2f adjustMousePosition(ViewProperties props, float aspectRatio,
		float x, float y) {
	x = x - props.offsetX;
	x = x * 2.0 / ((float) props.width);
	x -= 1.0;
	x *= aspectRatio;
	x = cullToRange(x, aspectRatio);

	y = y - props.offsetY;
	y = y * 2.0 / ((float) props.height);
	y -= 1.0;
	y *= -1.0;
	y = cullToRange(y, 1.0);

	return Math::Vector2f(x, y);
}

int main() {
	GameMemory memory = initGameMemory();

	GraphicsData graphics = initGraphicsData();
	AudioData audio = initAudioData();
	gameCode = loadGameCode();

	isRunning = true;
	bool wasLeftMousePressed = false;
	bool wasRightMousePressed = false;

	Input input[2] = { };
	Input *oldInput = &input[0];
	Input *newInput = &input[1];

	XEvent event;
	while (isRunning) {
		timespec startTime = { };
		clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);

		*newInput = *oldInput;

		bool mouseEvent = false;
		while (XEventsQueued(graphics.display,
		QueuedAfterReading)) {
			XNextEvent(graphics.display, &event);
			if (event.type == ClientMessage) {
				exitGame();
			}
			if (event.type == KeyPress || event.type == KeyRelease) {
				handleKeyEvent(&memory, &graphics, newInput, event.xkey);
			}
			if (event.type == ButtonPress || event.type == ButtonRelease) {
				handleMouseEvent(newInput, event.xbutton, &wasLeftMousePressed,
						&wasRightMousePressed);
				mouseEvent = true;
			}
			if (event.type == MotionNotify) {
				newInput->mousePosition = adjustMousePosition(
						graphics.viewProps, memory.aspectRatio, event.xmotion.x,
						event.xmotion.y);
			}
			if (event.type == ConfigureNotify) {
				XConfigureEvent xce = event.xconfigure;
				graphics.windowWidth = xce.width;
				graphics.windowHeight = xce.height;
				resizeViewport(&graphics, &memory);
			}
		}

		if (!isRunning) {
			break;
		}

		if (!mouseEvent) {
			newInput->shootKeyPressed = wasLeftMousePressed
					|| wasRightMousePressed;
			newInput->shootKeyClicked = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gameCode.updateAndRender(newInput, &memory);

		glXSwapBuffers(graphics.display, graphics.window);
		XSync(graphics.display, false);

		if (memory.doResize) {
			resizeViewport(&graphics, &memory);
		}

#if SOUND_ENABLE
		swapSoundBuffers(&memory);
#endif

		struct stat libraryStatBuffer = { };
		stat("./librth.so", &libraryStatBuffer);

		bool libraryNeedsToBeReloaded = (libraryStatBuffer.st_mtime
				!= gameCode.libraryMTime);
		if (libraryNeedsToBeReloaded) {
			unloadGameCode(&gameCode);
			gameCode = loadGameCode();
		}

		unsigned amountOfShaders = sizeof(memory.shaderFileNames)
				/ sizeof(memory.shaderFileNames[0]);
		for (unsigned i = 0; i < amountOfShaders; i++) {
			struct stat shaderStatBuffer = { };
			stat(memory.shaderFileNames[i], &shaderStatBuffer);
			memory.shaderModTimes[i][0] = memory.shaderModTimes[i][1];
			memory.shaderModTimes[i][1] = shaderStatBuffer.st_mtime;
		}

		Input *tmp = oldInput;
		oldInput = newInput;
		newInput = tmp;

		correctTiming(&graphics, startTime, false);
	}

	free(memory.permanent);
	free(memory.temporary);

// Clean up audio
	snd_pcm_drain(audio.pcm_handle);
	snd_pcm_close(audio.pcm_handle);
	free(audio.buffer);

	XCloseDisplay(graphics.display);
	return 0;
}
