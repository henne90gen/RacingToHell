#include <stdio.h>
#include <stdlib.h>
#include <X11/cursorfont.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include "RacingToHell.h"

Window createWindow(Display* display, int x, int y, unsigned width,
		unsigned height) {
	int screen = DefaultScreen(display);
	Visual* visual = DefaultVisual(display, screen);
	int depth = DefaultDepth(display, screen);
	XSetWindowAttributes attributes = { };
	attributes.background_pixel = XWhitePixel(display, screen);
	int border_width = 5;
	return XCreateWindow(display, XRootWindow(display, screen), x, y, width,
			height, border_width, depth, InputOutput, visual, CWBackPixel,
			&attributes);
}

int main() {
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	};
	int screen = DefaultScreen(display);
	int depth = DefaultDepth(display, screen);
	GC gc = XDefaultGC(display, screen);

	int x = 200;
	int y = 200;
	int width = 350;
	int height = 200;
	Window window = createWindow(display, x, y, width, height);
	XSelectInput(display, window, ExposureMask | KeyPressMask);

	Pixmap pixmap = XCreatePixmap(display, window, width, height, depth);
	char* buffer = (char*) malloc(4 * width * height);

	Visual* visual = DefaultVisual(display, screen);
	XImage *image = XCreateImage(display, visual, depth, ZPixmap, 0, buffer,
			width, height, 32, 0);

	XDefineCursor(display, window, XCreateFontCursor(display, XC_box_spiral));
	XMapWindow(display, window);

	XEvent e;
	while (1) {
		while (XCheckMaskEvent(display, ExposureMask | KeyPressMask, &e)) {
			if (e.type == Expose) {
				printf("Exposing\n");
			}
			if (e.type == KeyPress) {
				printf("Button %d pressed\n", e.xkey.keycode);
				if (e.xbutton.button == 9) {
					printf("Exiting\n");
					XFreePixmap(display, pixmap);
					XCloseDisplay(display);
					return 0;
				}
			}
		}

		VideoBuffer videoBuffer = { };
		videoBuffer.bytesPerPixel = 4;
		videoBuffer.width = width;
		videoBuffer.height = height;
		videoBuffer.content = buffer;

		updateAndRender(&videoBuffer);

		XPutImage(display, pixmap, gc, image, 0, 0, 0, 0, width, height);
		XCopyArea(display, pixmap, window, gc, 0, 0, width, height, 0, 0);
	}

	XFreePixmap(display, pixmap);
	XCloseDisplay(display);
	return 0;
}
