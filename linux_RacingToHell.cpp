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
	int x = 200;
	int y = 200;
	int border_width = 5;
	Visual* visual = DefaultVisual(display, screen);
	XSetWindowAttributes attributes = { };
	attributes.background_pixel = XWhitePixel(display, screen);
	graphics.window = XCreateWindow(display, XRootWindow(display, screen), x, y,
			videoBuffer.width, videoBuffer.height, border_width, depth,
			InputOutput, visual,
			CWBackPixel, &attributes);

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

void getRange(char* input, char* output, int startIndex, int endIndex) {
	for (int i = 0; i < endIndex - startIndex; i++) {
		output[i] = input[startIndex + i];
	}
}

void readImageFile(Texture* texture, char* fileName) {
	printf("Reading %s\n", fileName);

	FILE *f = fopen(fileName, "rb");
	if (!f) {
		fprintf(stderr, "Could not open file '%s'\n", fileName);
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	long fileSize = ftell(f);
	rewind(f);

	char* content = (char*) malloc(fileSize + 1);
	fread(content, fileSize, 1, f);
	fclose(f);

	content[fileSize] = 0; // 0-terminated
	if (content[0] != 'B' || content[1] != 'M') {
		fprintf(stderr, "Not a .bmp file.");
		exit(1);
	}

	// Check file size
	char* reader = (char*) malloc(4);
	getRange(content, reader, 2, 6);
	if (*((int*) reader) != fileSize) {
		fprintf(stderr, "Corrupt image file");
		exit(1);
	}

	int fileHeaderSize = 14;
	getRange(content, reader, fileHeaderSize, 18);
	int bitmapHeaderSize = *((int*) reader);

	getRange(content, reader, 18, 22);
	int width = *((int*) reader);

	getRange(content, reader, 22, 26);
	int height = *((int*) reader);

	reader = (char*) malloc(2);
	getRange(content, reader, 26, 28);
	short colorPlanes = *((short*) reader);
	getRange(content, reader, 28, 30);
	short bytesPerPixel = *((short*) reader);

	long int pixelArraySize = fileSize - (fileHeaderSize + bitmapHeaderSize);
	reader = (char*) malloc(pixelArraySize);
	getRange(content, reader, fileHeaderSize + bitmapHeaderSize, fileSize);

	texture->width = width;
	texture->height = height;
	texture->bytesPerPixel = bytesPerPixel;
	texture->content = malloc(pixelArraySize);
	getRange(reader, (char*) texture->content, 0, pixelArraySize);
	printf("Successfully loaded image %s.\n", fileName);
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

		updateAndRender(&graphics.videoBuffer);

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
