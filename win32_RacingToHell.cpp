#include <Windows.h>
#include <stdlib.h>
#include <iostream>

#include "RacingToHell.h"

struct win32_offScreenBuffer
{
	BITMAPINFO info;
	
	unsigned width, height, bytesPerPixel;
	void *content;
};

static bool isRunning = true;
static win32_offScreenBuffer buffer;

void resizeOffscreenBuffer(win32_offScreenBuffer *buffer, unsigned width, unsigned height)
{
	if (buffer->content)
	{
		VirtualFree(buffer->content, 0, MEM_RELEASE);
	}
	
	buffer->width = width;
	buffer->height = height;
	buffer->bytesPerPixel = 4;
	
	unsigned size = width * height * buffer->bytesPerPixel;
	
	buffer->info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	buffer->info.bmiHeader.biWidth = width;
	buffer->info.bmiHeader.biHeight = -height;
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = 32;
	buffer->info.bmiHeader.biCompression = BI_RGB;
	
	buffer->content = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void drawBuffer(HDC deviceContext, win32_offScreenBuffer *buffer)
{
	int result = StretchDIBits(
		deviceContext,
		0, 0, buffer->width, buffer->height,
		0, 0, buffer->width, buffer->height,
		buffer->content,
		&buffer->info,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	
	if (result == 0)
	{
		std::cout << "DIBits failed." << std::endl;
	}
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
		
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			
			HDC deviceContext = BeginPaint(windowHandle, &paint);
			drawBuffer(deviceContext, &buffer);
			EndPaint(windowHandle, &paint);
		} break;
	}
	
	return DefWindowProc(windowHandle, message, wParam, lParam);
}  

HWND openWindow(HINSTANCE instance, int show)
{
    WNDCLASS windowClass = {};
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hIcon = LoadIcon(0, IDI_WINLOGO);
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "Window Class";

    RegisterClass(&windowClass);

    HWND windowHandle = CreateWindowEx(
        0,
        windowClass.lpszClassName,
        "Racing To Hell",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        instance,
        NULL
        );

    if(!windowHandle) {
        std::cout << "Failed to create window" << std::endl;
		exit(1);
	}

    //Show the window
    ShowWindow(windowHandle, show);

	return windowHandle;
}

void handleKeyStroke(WPARAM keyCode, LPARAM flags)
{
	if (keyCode == VK_ESCAPE)
	{
		isRunning = false;
	}
}

void drawSomething(win32_offScreenBuffer *buffer)
{
	for (unsigned y = 0; y < buffer->height; ++y)
	{
		for (unsigned x = 0; x < buffer->width; ++x)
		{
			//AARRGGBB
			((int *)buffer->content)[buffer->width * y + x] = (255 << 16); 
		}
	}
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR args, int show)
{
	resizeOffscreenBuffer(&buffer, 1280, 720);
	drawSomething(&buffer);
	
	HWND windowHandle = openWindow(instance, show);
	
	while (isRunning)
	{
		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{	
			if (message.message == WM_QUIT)
			{
				isRunning = false;
			}
			else if (message.message == WM_KEYDOWN || message.message == WM_KEYUP)
			{
				handleKeyStroke(message.wParam, message.lParam);
			}
			else
			{
				TranslateMessage(&message); 
				DispatchMessage(&message);
			}
		}
		
		videoBuffer vBuffer;
		vBuffer.width = buffer.width;
		vBuffer.height = buffer.height;
		vBuffer.bytesPerPixel = buffer.bytesPerPixel;
		vBuffer.content = buffer.content;
		
		updateAndRender(&vBuffer);
		
		HDC deviceContext = GetDC(windowHandle);
		drawBuffer(deviceContext, &buffer);
		ReleaseDC(windowHandle, deviceContext);
	}
	
	return 0;
}
