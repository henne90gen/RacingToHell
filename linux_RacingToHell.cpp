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

struct GraphicsData {
	Display* display;
	Window window;
	GC gc;
	Pixmap pixmap;
	XImage* image;
	VideoBuffer videoBuffer;
};

static bool isRunning;
static GraphicsData graphics;

GraphicsData initGraphicsData(Display *display) {
	GraphicsData graphics = { };
	graphics.display = display;
	VideoBuffer videoBuffer = { };
	videoBuffer.width = 400;
	videoBuffer.height = 300;
	videoBuffer.bytesPerPixel = 4;
	videoBuffer.content = malloc(
			videoBuffer.bytesPerPixel * videoBuffer.width * videoBuffer.height);
	graphics.videoBuffer = videoBuffer;

	int screen = DefaultScreen(display);
	int depth = DefaultDepth(display, screen);
	int x = 500;
	int y = 300;
	int border_width = 5;
	Visual* visual = DefaultVisual(display, screen);
	XSetWindowAttributes attributes = { };
	attributes.background_pixel = XWhitePixel(display, screen);
	graphics.window = XCreateWindow(display, XRootWindow(display, screen), x, y,
			videoBuffer.width, videoBuffer.height, border_width, depth,
			InputOutput, visual, CWBackPixel, &attributes);

	XSelectInput(display, graphics.window,
	KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
	XDefineCursor(display, graphics.window,
			XCreateFontCursor(display, XC_box_spiral));
	XMapWindow(display, graphics.window);

	graphics.gc = XDefaultGC(display, screen);

	graphics.pixmap = XCreatePixmap(display, graphics.window,
			graphics.videoBuffer.width, graphics.videoBuffer.height, depth);
	graphics.image = XCreateImage(display, visual, depth, ZPixmap, 0,
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
		printf("Mouse button pressed!\n");
	} else {
		printf("Mouse button released!\n");
	}

	switch (event.button) {
	case Button1:
		printf("");
		break;
	case Button2:
		printf("");
		break;
	case Button3:
		printf("");
		break;
	case Button4:
		printf("");
		break;
	case Button5:
		printf("");
		break;
	}
}

int main() {
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	};

	isRunning = true;
	graphics = initGraphicsData(display);

	Input input[2];
	Input *oldInput = &input[0];
	Input *newInput = &input[1];

	XEvent event;
	while (isRunning) {
		timespec startTime = { };
		clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);
		*newInput = *oldInput;

		while (XCheckMaskEvent(display,
				Expose | KeyPressMask | KeyReleaseMask | ButtonPressMask
						| ButtonReleaseMask, &event)) {
			if (event.type == Expose) {
				printf("Exposing\n");
			}
			if (event.type == KeyPress || event.type == KeyRelease) {
				handleKeyEvent(display, newInput, event.xkey);
			}
			if (event.type == ButtonPress || event.type == ButtonRelease) {
				handleMouseEvent(newInput, event.xbutton);
			}
		}

		updateAndRender(&graphics.videoBuffer, newInput);

		// swapping buffer
		XPutImage(display, graphics.pixmap, graphics.gc, graphics.image, 0, 0,
				0, 0, graphics.videoBuffer.width, graphics.videoBuffer.height);
		XCopyArea(display, graphics.pixmap, graphics.window, graphics.gc, 0, 0,
				graphics.videoBuffer.width, graphics.videoBuffer.height, 0, 0);

		Input *tmp = oldInput;
		oldInput = newInput;
		newInput = tmp;

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

	XFreePixmap(display, graphics.pixmap);
	XCloseDisplay(display);
	return 0;
}
