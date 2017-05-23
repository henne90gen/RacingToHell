#include <stdio.h>
#include <stdlib.h>
#include <X11/cursorfont.h>
#include <X11/X.h>
#include <X11/Xlib.h>

#include "RacingToHell.h"

struct GraphicsData {
	Display* display;
	Window window;
	GC gc;
	Pixmap pixmap;
	XImage* image;
	VideoBuffer videoBuffer;
};

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

	XSelectInput(display, graphics.window, ExposureMask | KeyPressMask);
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

	File file = {};
	file.size = fileSize;
	file.content = content;

	return file;
}

int main() {
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	};

	graphics = initGraphicsData(display);

	XEvent e;
	while (1) {
		while (XCheckMaskEvent(display, ExposureMask | KeyPressMask, &e)) {
			if (e.type == Expose) {
				printf("Exposing\n");
			}
			if (e.type == KeyPress) {
				printf("Button %d pressed\n", e.xkey.keycode);
				if (e.xbutton.button == 9) { // Escape pressed
					printf("Exiting\n");
					XFreePixmap(display, graphics.pixmap);
					XCloseDisplay(display);
					return 0;
				}
			}
		}

		Input input = { };

		updateAndRender(&graphics.videoBuffer, &input);

		// swapping buffer
		XPutImage(display, graphics.pixmap, graphics.gc, graphics.image, 0, 0,
				0, 0, graphics.videoBuffer.width, graphics.videoBuffer.height);
		XCopyArea(display, graphics.pixmap, graphics.window, graphics.gc, 0, 0,
				graphics.videoBuffer.width, graphics.videoBuffer.height, 0, 0);
	}

	XFreePixmap(display, graphics.pixmap);
	XCloseDisplay(display);
	return 0;
}
