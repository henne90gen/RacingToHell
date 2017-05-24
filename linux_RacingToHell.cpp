#include <stdio.h>
#include <stdlib.h>
#include <X11/cursorfont.h>
#include <X11/X.h>
#include <X11/Xlib.h>

#include "platform.h"
#include "RacingToHell.h"

#define XK_W 25
#define XK_A 38
#define XK_S 39
#define XK_D 40
#define XK_Space 65
#define XM_Left Button1
#define XM_Right Button3
#define XM_Middle Button2 // FIXME is this really mouse middle?
#define XM_ScrollUp Button4
#define XM_ScrollDown Button5

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

GraphicsData initGraphicsData() {
	GraphicsData graphics = { };
	graphics.display = XOpenDisplay(NULL);
	if (graphics.display == NULL) {
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	};

	VideoBuffer videoBuffer = { };
	videoBuffer.width = 400;
	videoBuffer.height = 300;
	videoBuffer.bytesPerPixel = 4;
	videoBuffer.content = malloc(
			videoBuffer.bytesPerPixel * videoBuffer.width * videoBuffer.height);
	graphics.videoBuffer = videoBuffer;

	int screen = DefaultScreen(graphics.display);
	int depth = DefaultDepth(graphics.display, screen);
	int x = 500;
	int y = 300;
	int border_width = 5;
	Visual* visual = DefaultVisual(graphics.display, screen);
	XSetWindowAttributes attributes = { };
	attributes.background_pixel = XWhitePixel(graphics.display, screen);
	graphics.window = XCreateWindow(graphics.display,
			XRootWindow(graphics.display, screen), x, y, videoBuffer.width,
			videoBuffer.height, border_width, depth,
			InputOutput, visual, CWBackPixel, &attributes);

	XSelectInput(graphics.display, graphics.window, EVENTS_MASK);
	XDefineCursor(graphics.display, graphics.window,
			XCreateFontCursor(graphics.display, XC_arrow));
	XMapWindow(graphics.display, graphics.window);

	Atom wmDeleteWindow = XInternAtom(graphics.display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(graphics.display, graphics.window, &wmDeleteWindow, 1);

	graphics.gc = XDefaultGC(graphics.display, screen);

	graphics.pixmap = XCreatePixmap(graphics.display, graphics.window,
			graphics.videoBuffer.width, graphics.videoBuffer.height, depth);
	graphics.image = XCreateImage(graphics.display, visual, depth, ZPixmap, 0,
			(char *) graphics.videoBuffer.content, graphics.videoBuffer.width,
			graphics.videoBuffer.height, 32, 0);
	return graphics;
}

File readFile(char* fileName) {
	printf("Reading %s\n", fileName);

	FILE *f = fopen(fileName, "rb");
	if (!f) {
		fprintf(stderr, "Could not open file '%s'\n", fileName);
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	long int fileSize = ftell(f);
	rewind(f);

	char* content = (char*) malloc(fileSize + 1);
	fread(content, fileSize, 1, f);
	fclose(f);

	content[fileSize] = 0; // 0-terminated

	File file = { };
	file.size = fileSize;
	file.name = fileName;
	file.content = content;

	return file;
}

void handleKeyEvent(Display* display, Input* input, XKeyEvent event) {
	if (event.keycode == 9) { // Escape pressed
		printf("Exiting\n");
		isRunning = false;
		return;
	}

	bool keyPressed = event.type == KeyPress;
	if (!keyPressed) {
		while (XEventsQueued(display, QueuedAfterReading)) {
			XEvent nextEvent;
			XPeekEvent(display, &nextEvent);

			// FIXME getting a lot of NoExpose for some reason
			if (nextEvent.type == NoExpose) {
				XNextEvent(display, &nextEvent);
				continue;
			}

			if (nextEvent.type == KeyPress && nextEvent.xkey.time == event.time
					&& nextEvent.xkey.keycode == event.keycode) {
				// key wasn't acutally released
				return;
			}
		}
	}

	switch (event.keycode) {
	case XK_W:
		input->upKey = keyPressed;
		printf("Pressed W\n");
		break;
	case XK_A:
		input->leftKey = keyPressed;
		printf("Pressed A\n");
		break;
	case XK_S:
		input->downKey = keyPressed;
		printf("Pressed S\n");
		break;
	case XK_D:
		input->rightKey = keyPressed;
		printf("Pressed D\n");
		break;
	case XK_Space:
		input->shootKey = keyPressed;
		printf("Pressed Space\n");
		break;
	}

	if (keyPressed) {
		printf("Button %d pressed\n", event.keycode);
	} else {
		printf("Button %d released\n", event.keycode);
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
	case XM_Left:
		input->shootKey = buttonPressed;
		break;
	case XM_Middle:
		break;
	case XM_Right:
		input->shootKey = buttonPressed;
		break;
	case XM_ScrollUp:
		break;
	case XM_ScrollDown:
		break;
	}
}

void correctTiming(timespec startTime) {
	timespec endTime = { };
	clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
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
	float secondsElapsed = (endTime.tv_nsec - startTime.tv_nsec)
			/ 1000000000.0f;
	//		printf("Frametime: %f, Framerate: %f\n", secondsElapsed,
	//				1.0f / secondsElapsed);
}

int main() {
	graphics = initGraphicsData();
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

		updateAndRender(&graphics.videoBuffer, newInput);

		// swapping buffer
		XPutImage(graphics.display, graphics.pixmap, graphics.gc,
				graphics.image, 0, 0, 0, 0, graphics.videoBuffer.width,
				graphics.videoBuffer.height);
		XCopyArea(graphics.display, graphics.pixmap, graphics.window,
				graphics.gc, 0, 0, graphics.videoBuffer.width,
				graphics.videoBuffer.height, 0, 0);

		Input *tmp = oldInput;
		oldInput = newInput;
		newInput = tmp;

		correctTiming(startTime);
	}

	XFreePixmap(graphics.display, graphics.pixmap);
	XCloseDisplay(graphics.display);
	return 0;
}
