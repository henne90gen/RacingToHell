#include <bits/types/struct_timespec.h>
#include <stdlib.h>
#include <sys/time.h>
#include <X11/cursorfont.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <cstdio>
#include <ctime>
#include <string>
#include <sstream>

#include "platform.h"
#include "RacingToHell.h"

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
	GC gc;
	Pixmap pixmap;
	XImage* image;
	VideoBuffer videoBuffer;
};

long int EVENTS_MASK = KeyPressMask | KeyReleaseMask | ButtonPressMask
		| ButtonReleaseMask | PointerMotionMask;
static bool isRunning;
static GraphicsData graphics;
Pixmap icon_pixmap;

GraphicsData initGraphicsData(GameMemory *memory) {
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

	int screen = DefaultScreen(graphics.display);
	int depth = DefaultDepth(graphics.display, screen);
	int x = 0;
	int y = 0;
	int border_width = 0;
	graphics.window = XCreateSimpleWindow(graphics.display,
			RootWindow(graphics.display, screen), x, y, videoBuffer.width,
			videoBuffer.height, border_width,
			BlackPixel(graphics.display, screen),
			WhitePixel(graphics.display, screen));

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
			*dst++ = *src++;
		}
	}
	XChangeProperty(graphics.display, graphics.window, iconAtom, XA_CARDINAL,
			32, PropModeReplace, (unsigned char *) propdata, propsize);

	// subscribe to events
	XSelectInput(graphics.display, graphics.window, EVENTS_MASK);

	// change cursor
	XDefineCursor(graphics.display, graphics.window,
			XCreateFontCursor(graphics.display, XC_arrow));

	XMapWindow(graphics.display, graphics.window);

	Atom wmDeleteWindow = XInternAtom(graphics.display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(graphics.display, graphics.window, &wmDeleteWindow, 1);

	graphics.image = XCreateImage(graphics.display,
			XDefaultVisual(graphics.display, screen), depth, ZPixmap, 0,
			(char *) graphics.videoBuffer.content, graphics.videoBuffer.width,
			graphics.videoBuffer.height, 32, 0);
	graphics.pixmap = XCreatePixmap(graphics.display, graphics.window,
			graphics.videoBuffer.width, graphics.videoBuffer.height, depth);
	graphics.gc = XDefaultGC(graphics.display, screen);

	XFlush(graphics.display);
	return graphics;
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
		input->shootKey = keyPressed;
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
		input->shootKey = buttonPressed;
		break;
	case MouseMiddle:
		break;
	case MouseRight:
		input->shootKey = buttonPressed;
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
	XStoreName(graphics.display, graphics.window, ss.str().c_str());
	XFlush(graphics.display);

	if (consoleOutput) {
		printf("Frametime: %fms, Framerate: %f\n",
				nanoSecondsElapsed / 1000000.0f,
				1000000000.0f / nanoSecondsElapsed);
	}
}

int main() {
	GameMemory memory;
	memory.temporaryMemorySize = 10 * 1024 * 1024;
	memory.permanentMemorySize = 100 * 1024 * 1024;
	memory.temporary = (char*) malloc(memory.temporaryMemorySize);
	memory.permanent = (char*) malloc(memory.permanentMemorySize);

	graphics = initGraphicsData(&memory);
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
				newInput->mouseX = event.xmotion.x;
				newInput->mouseY = event.xmotion.y;
			}
		}

		if (!isRunning) {
			break;
		}

		updateAndRender(&graphics.videoBuffer, newInput, &memory);

		// swapping buffer
		XPutImage(graphics.display, graphics.pixmap, graphics.gc,
				graphics.image, 0, 0, 0, 0, graphics.videoBuffer.width,
				graphics.videoBuffer.height);
		XCopyArea(graphics.display, graphics.pixmap, graphics.window,
				graphics.gc, 0, 0, graphics.videoBuffer.width,
				graphics.videoBuffer.height, 0, 0);
		XSync(graphics.display, false);

		Input *tmp = oldInput;
		oldInput = newInput;
		newInput = tmp;

		correctTiming(startTime, false);
	}

	XCloseDisplay(graphics.display);
	return 0;
}
