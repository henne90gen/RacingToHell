#include "linux_RacingToHell.h"

AudioData initAudioData() {
	// FIXME make error messages more descriptive
	printf("Loading audio context.\n");

	AudioData audio = { };

	char *pcm_name;
	pcm_name = strdup("default");
	int error;
	if ((error = snd_pcm_open(&audio.pcm_handle, pcm_name,
			SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0) {
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

	if ((error = snd_pcm_hw_params_set_rate_near(audio.pcm_handle,
			audio.hw_params, &audio.sample_rate, 0)) < 0) {
		std::string errorMsg = "Cannot set sample rate. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	if ((error = snd_pcm_hw_params(audio.pcm_handle, audio.hw_params)) < 0) {
		std::string errorMsg = "Cannot set hardware parameters. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	printf("PCM name: %s\n", snd_pcm_name(audio.pcm_handle));
	printf("PCM state: %s\n",
			snd_pcm_state_name(snd_pcm_state(audio.pcm_handle)));

	snd_pcm_hw_params_get_channels(audio.hw_params, &audio.channels);
	printf("Channels: %i\n", audio.channels);

	snd_pcm_hw_params_get_rate(audio.hw_params, &audio.sample_rate, 0);
	printf("Rate: %i\n", audio.sample_rate);

	if ((error = snd_pcm_hw_params_set_period_size(audio.pcm_handle,
			audio.hw_params, audio.frames, 0)) < 0) {
		std::string errorMsg = "Couldn't set period size. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	if ((error = snd_pcm_hw_params_get_period_size(audio.hw_params,
			&audio.frames, 0)) < 0) {
		std::string errorMsg = "Couldn't get period size. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	printf("Period size: %i\n", audio.frames);

	if ((error = snd_pcm_hw_params_get_period_time(audio.hw_params,
			&audio.period_time, NULL)) < 0) {
		std::string errorMsg = "Couldn't get period time. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}

	audio.buff_size = audio.frames * audio.channels;
	audio.buffer = (int16_t *) malloc(audio.buff_size * 2); // 2 bytes per sample

	// Example on how to output sound
	// Outputs a sine wave
//	float sinebuff[48] = { 0, 4276, 8480, 12539, 16383, 19947, 23169, 25995,
//			28377, 30272, 31650, 32486, 32767, 32486, 31650, 30272, 28377,
//			25995, 23169, 19947, 16383, 12539, 8480, 4276, 0, -4276, -8480,
//			-12539, -16383, -19947, -23169, -25995, -28377, -30272, -31650,
//			-32486, -32767, -32486, -31650, -30272, -28377, -25995, -23169,
//			-19947, -16383, -12539, -8480, -4276 };
//	unsigned phase = 0;
//	int seconds = 1;
//	for (int loops = (seconds * 1000000) / audio.period_time; loops > 0;
//			loops--) {
//
//		for (unsigned i = 0; i < audio.buff_size; i++) {
//			audio.buffer[i] = sinebuff[phase % 48];
//			phase = (phase + 1) % 4800;
//		}
//
//		if ((error = snd_pcm_writei(audio.pcm_handle, audio.buffer,
//				audio.frames)) == -EPIPE) {
//			printf("Underrun occurred.\n");
//			snd_pcm_prepare(audio.pcm_handle);
//		} else if (error < 0) {
//			std::string errorMsg = "Can't write to PCM device. "
//					+ std::string(snd_strerror(error));
//			abort(errorMsg);
//		}
//	}

	printf("Successfully loaded audio context.\n");
	return audio;
}

void setupOpenGL(VideoBuffer *videoBuffer);

GraphicsData initGraphicsData() {
	GraphicsData graphics = { };
	graphics.display = XOpenDisplay(NULL);
	if (!graphics.display) {
		abort("Cannot open display.");
	};

	VideoBuffer videoBuffer = { };
	videoBuffer.width = WINDOW_WIDTH;
	videoBuffer.height = WINDOW_HEIGHT;
	videoBuffer.bytesPerPixel = 4;
	videoBuffer.content = malloc(
			videoBuffer.bytesPerPixel * videoBuffer.width * videoBuffer.height);
	graphics.videoBuffer = videoBuffer;

	Window root = DefaultRootWindow(graphics.display);
	graphics.vi = glXChooseVisual(graphics.display, 0, att);
	if (!graphics.vi) {
		abort("No appropriate visual found.");
	}
	graphics.cmap = XCreateColormap(graphics.display, root, graphics.vi->visual,
	AllocNone);

	graphics.swa.colormap = graphics.cmap;
	graphics.swa.event_mask = EVENT_MASK;
	graphics.window = XCreateWindow(graphics.display, root, 0, 0, 600, 800, 0,
			graphics.vi->depth, InputOutput, graphics.vi->visual,
			CWColormap | CWEventMask, &graphics.swa);

	graphics.glc = glXCreateContext(graphics.display, graphics.vi, NULL,
	GL_TRUE);
	glXMakeCurrent(graphics.display, graphics.window, graphics.glc);
	glEnable(GL_DEPTH_TEST);

	XGetWindowAttributes(graphics.display, graphics.window, &graphics.gwa);
	glViewport(0, 0, graphics.gwa.width, graphics.gwa.height);

	if (glewInit() != GLEW_OK) {
		abort("Failed to initialize GLEW.");
	}

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
	File file = readFile("./res/icon.bmp");
	GameMemory bmpMemory = { };
	char bmpBuffer[48 * 48 * 4];
	bmpMemory.permanent = bmpBuffer;
	bmpMemory.permanentMemorySize = sizeof(bmpBuffer);
	Texture texture = readBmpIntoMemory(file, &bmpMemory);
	int propsize = 2 + (texture.width * texture.height);
	long *propdata = (long*) malloc(propsize * sizeof(long));

	propdata[0] = texture.width;
	propdata[1] = texture.height;
	uint32_t *src;
	long *dst = &propdata[2];
	for (unsigned y = 0; y < texture.height; ++y) {
		src =
				(uint32_t*) ((uint8_t*) texture.content
						+ y * (texture.width * 4));
		for (unsigned x = 0; x < texture.width; ++x) {
			uint32_t color = *src++;
			uint8_t alpha = (color & 0xff000000) >> 24;
			uint8_t red = (color & 0x000000ff);
			uint8_t green = (color & 0x0000ff00) >> 8;
			uint8_t blue = (color & 0x00ff0000) >> 16;
			color = (alpha << 24) + (red << 16) + (green << 8) + blue;
			*dst++ = color;
		}
	}
	XChangeProperty(graphics.display, graphics.window, iconAtom,
	XA_CARDINAL, 32, PropModeReplace, (unsigned char *) propdata, propsize);

	// subscribe to events
	XSelectInput(graphics.display, graphics.window, EVENT_MASK);

	// change cursor
	XDefineCursor(graphics.display, graphics.window,
			XCreateFontCursor(graphics.display, XC_arrow));

	Atom wmDeleteWindow = XInternAtom(graphics.display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(graphics.display, graphics.window, &wmDeleteWindow, 1);

	XMapWindow(graphics.display, graphics.window);
	XFlush(graphics.display);

	setupOpenGL(&videoBuffer);

	return graphics;
}

void abort(std::string message) {
	fprintf(stderr, "%s", (message + "\n").c_str());
	exit(1);
}

File readFile(std::string fileName) {
	printf("Reading %s\n", fileName.c_str());

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

void freeFile(File *file) {
	if (file->content) {
		free(file->content);
	}

	file->size = 0;
}

void handleKeyEvent(Display* display, Input* input, XKeyEvent event) {
	if (event.keycode == 9) { // Escape pressed
		printf("Exiting\n");
		isRunning = false;
		return;
	}

	bool keyPressed = event.type == KeyPress;
	switch (event.keycode) {
	case KeyW:
		input->upKey = keyPressed;
		break;
	case KeyA:
		input->leftKey = keyPressed;
		break;
	case KeyS:
		input->downKey = keyPressed;
		break;
	case KeyD:
		input->rightKey = keyPressed;
		break;
	case KeySpace:
		input->shootKeyPressed = keyPressed;
		break;
	}
}

void handleMouseEvent(Input* input, XButtonEvent event) {
	bool buttonPressed = event.type == ButtonPress;
	if (buttonPressed) {
		printf("Mouse button %d pressed!\n", event.button);
	} else {
		printf("Mouse button %d released!\n", event.button);
	}

	switch (event.button) {
	case MouseLeft:
		input->shootKeyPressed = buttonPressed;
		input->shootKeyClicked = buttonPressed;
		break;
	case MouseMiddle:
		break;
	case MouseRight:
		input->shootKeyPressed = buttonPressed;
		input->shootKeyClicked = buttonPressed;
		break;
	case MouseScrollUp:
		break;
	case MouseScrollDown:
		break;
	}
}

void correctTiming(timespec startTime, bool consoleOutput) {
	timespec endTime = { };
	clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
	if (endTime.tv_nsec < startTime.tv_nsec) {
		return;
	}
	long nanoSecondsElapsed = endTime.tv_nsec - startTime.tv_nsec;

	// INFO OpenGL runs at exactly 60 FPS, no sleep necessary
	//	const float targetFrameTime = 1000000000.0f / 60.0f;
//	if (nanoSecondsElapsed < targetFrameTime) {
//		timespec sleepTime = { };
//		sleepTime.tv_sec = (targetFrameTime - nanoSecondsElapsed)
//				/ 1000000000.0f;
//		sleepTime.tv_nsec = targetFrameTime - nanoSecondsElapsed;
////		nanosleep(&sleepTime, NULL);
//	}
//	clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
//	if (endTime.tv_nsec < startTime.tv_nsec) {
//		return;
//	}
//	nanoSecondsElapsed = endTime.tv_nsec - startTime.tv_nsec;

	std::stringstream ss;
	ss << WINDOW_TITLE << ": "
			<< std::to_string(nanoSecondsElapsed / 1000000.0f);
	ss << "ms, " << std::to_string(1000000000.0f / nanoSecondsElapsed);
	ss << " FPS";
	XStoreName(graphics.display, graphics.window, ss.str().c_str());
	XFlush(graphics.display);

	if (consoleOutput) {
		printf("Frametime: %fms, Framerate: %f\n",
				nanoSecondsElapsed / 1000000.0f,
				1000000000.0f / nanoSecondsElapsed);
	}
}

void setTexturePixels(GLuint texture_object_id, VideoBuffer *videoBuffer) {
	glBindTexture(GL_TEXTURE_2D, texture_object_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, videoBuffer->width,
			videoBuffer->height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, videoBuffer->content);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint loadTexture(VideoBuffer * videoBuffer) {
	GLuint texture_object_id;
	glGenTextures(1, &texture_object_id);

	setTexturePixels(texture_object_id, videoBuffer);
	return texture_object_id;
}

GLuint compileShader(const GLenum type, const GLchar *source,
		const GLint length) {
	GLuint shader_object_id = glCreateShader(type);
	GLint compile_status;

	glShaderSource(shader_object_id, 1, (const GLchar **) &source, &length);
	glCompileShader(shader_object_id);
	glGetShaderiv(shader_object_id, GL_COMPILE_STATUS, &compile_status);

	if (compile_status == GL_FALSE) {
		int infoLogLength;
		glGetShaderiv(shader_object_id, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shader_object_id, infoLogLength, NULL,
					&VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}
		std::string message = "Failed to compile shader. "
				+ std::to_string(type) + "\n" + std::string(source);
		abort(message);
	}

	return shader_object_id;
}

GLuint linkProgram(const GLuint vertex_shader, const GLuint fragment_shader) {
	GLuint program_object_id = glCreateProgram();
	GLint link_status;

	glAttachShader(program_object_id, vertex_shader);
	glAttachShader(program_object_id, fragment_shader);
	glLinkProgram(program_object_id);
	glGetProgramiv(program_object_id, GL_LINK_STATUS, &link_status);

	if (link_status == GL_FALSE) {
		abort("Couldn't link shader program.");
	}

	return program_object_id;
}

GLuint buildProgram() {

	// FIXME don't hard code the shaders
	const GLchar *vertex_shader_source =
			"attribute vec4 a_Position;attribute vec2 a_TextureCoordinates;varying vec2 v_TextureCoordinates;void main() {v_TextureCoordinates = a_TextureCoordinates;gl_Position = a_Position;}";
	const GLint vertex_shader_source_length = 179;
	const GLchar *fragment_shader_source =
			"uniform sampler2D u_TextureUnit;varying vec2 v_TextureCoordinates;void main() {gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);}";
	const GLint fragment_shader_source_length = 166;

	GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_shader_source,
			vertex_shader_source_length);
	GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER,
			fragment_shader_source, fragment_shader_source_length);
	return linkProgram(vertex_shader, fragment_shader);
}

GLuint buildProgram(const GLchar *vertex_shader_source,
		const GLint vertex_shader_source_length,
		const GLchar *fragment_shader_source,
		const GLint fragment_shader_source_length) {

	GLuint vertex_shader = compileShader(
	GL_VERTEX_SHADER, vertex_shader_source, vertex_shader_source_length);
	GLuint fragment_shader = compileShader(
	GL_FRAGMENT_SHADER, fragment_shader_source, fragment_shader_source_length);
	return linkProgram(vertex_shader, fragment_shader);
}

GLuint buildProgramFromAssets(const char *vertex_shader_path,
		const char *fragment_shader_path) {
	const File vertex_shader_source = readFile(vertex_shader_path);
	const File fragment_shader_source = readFile(fragment_shader_path);
	const GLuint program_object_id = buildProgram(vertex_shader_source.content,
			vertex_shader_source.size, fragment_shader_source.content,
			fragment_shader_source.size);

	return program_object_id;
}

GLuint createVertexBufferObject(const GLsizeiptr size, const GLvoid *data,
		const GLenum usage) {
	GLuint vbo_object;
	glGenBuffers(1, &vbo_object);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo_object;
}

void setupOpenGL(VideoBuffer *videoBuffer) {
	texture = loadTexture(videoBuffer);
	buffer = createVertexBufferObject(sizeof(rect), rect, GL_STATIC_DRAW);

	program = buildProgram();

	a_position_location = glGetAttribLocation(program, "a_Position");
	a_texture_coordinates_location = glGetAttribLocation(program,
			"a_TextureCoordinates");
	u_texture_unit_location = glGetUniformLocation(program, "u_TextureUnit");
}

void swapVideoBuffers(VideoBuffer *videoBuffer) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setTexturePixels(texture, videoBuffer);

	glUseProgram(program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(u_texture_unit_location, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(a_position_location, 2, GL_FLOAT, GL_FALSE,
			4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE,
			4 * sizeof(GL_FLOAT), BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(a_position_location);
	glEnableVertexAttribArray(a_texture_coordinates_location);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glXSwapBuffers(graphics.display, graphics.window);
	XSync(graphics.display, false);
}

void swapSoundBuffers(GameMemory *memory) {
	SoundOutputBuffer soundBuffer = { };
	soundBuffer.samplesPerSecond = audio.sample_rate;
	soundBuffer.sampleCount = audio.buff_size / audio.channels;
	soundBuffer.samples = audio.buffer;

	Sound::getSoundSamples(memory, &soundBuffer);

	int error;
	error = snd_pcm_writei(audio.pcm_handle, audio.buffer, audio.frames);
	if (error == -EPIPE) {
		printf("Underrun occurred.\n");
		snd_pcm_prepare(audio.pcm_handle);
	} else if (error < 0) {
		std::string errorMsg = "Can't write to PCM device. "
				+ std::string(snd_strerror(error));
		abort(errorMsg);
	}
}

int main() {
	GameMemory memory;
	memory.temporaryMemorySize = 10 * 1024 * 1024;
	memory.permanentMemorySize = 100 * 1024 * 1024;
	memory.temporary = (char*) malloc(memory.temporaryMemorySize);
	memory.permanent = (char*) malloc(memory.permanentMemorySize);

	graphics = initGraphicsData();
	audio = initAudioData();

	isRunning = true;

	Input input[2] = { };
	Input *oldInput = &input[0];
	Input *newInput = &input[1];

	XEvent event;
	while (isRunning) {
		timespec startTime = { };
		clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);
		*newInput = *oldInput;

		while (XEventsQueued(graphics.display, QueuedAfterReading)) {
			XNextEvent(graphics.display, &event);
			if (event.type == ClientMessage) {
				printf("Exiting\n");
				isRunning = false;
			}
			if (event.type == KeyPress || event.type == KeyRelease) {
				handleKeyEvent(graphics.display, newInput, event.xkey);
			}
			if (event.type == ButtonPress || event.type == ButtonRelease) {
				handleMouseEvent(newInput, event.xbutton);
			}
			if (event.type == MotionNotify) {
				Math::Vector2f mousePosition = { };
				mousePosition.x = event.xmotion.x;
				mousePosition.y = event.xmotion.y;
				newInput->mousePosition = mousePosition;
			}
		}

		if (!isRunning) {
			break;
		}

		updateAndRender(&graphics.videoBuffer, newInput, &memory);

		swapSoundBuffers(&memory);

		swapVideoBuffers(&graphics.videoBuffer);

		Input *tmp = oldInput;
		oldInput = newInput;
		newInput = tmp;

		correctTiming(startTime, false);
	}

	// Clean up audio
	snd_pcm_drain(audio.pcm_handle);
	snd_pcm_close(audio.pcm_handle);
	free(audio.buffer);

	XCloseDisplay(graphics.display);
	return 0;
}
