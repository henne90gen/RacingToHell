#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "RacingToHell.h"
#include "win32_RacingToHell.h"

static bool isRunning = true;
static OffscreenBuffer buffer;

void resizeOffscreenBuffer(OffscreenBuffer *buffer, unsigned width, unsigned height)
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

void drawBuffer(HDC deviceContext, OffscreenBuffer *buffer)
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
        case WM_CLOSE:
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

    ShowWindow(windowHandle, show);

	return windowHandle;
}

void handleKeyStroke(Input *input, WPARAM keyCode, LPARAM flags)
{
	if (keyCode == VK_ESCAPE)
	{
		isRunning = false;
        return;
	}

    bool wasKeyDown = (bool)(flags & (1 << 30));
    bool isKeyDown = !((bool)(flags & (1 << 31)));

    if (wasKeyDown != isKeyDown)
    {
        switch (keyCode)
        {
            case ('W'):
            {
                input->upKey = isKeyDown;
            } break;
        
            case ('A'):
            {
                input->leftKey = isKeyDown;
            } break;

            case ('S'):
            {
                input->downKey = isKeyDown;
            } break;

            case ('D'):
            {
                input->rightKey = isKeyDown;
            } break;

            case (VK_SPACE):
            {
                input->pauseKey = isKeyDown;
            } break;
        }
    }


}

void drawSomething(OffscreenBuffer *buffer)
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

File readFile(std::string filename)
{
    File result = {};

    HANDLE fileHandle = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER fileSize;

        if (GetFileSizeEx(fileHandle, &fileSize))
        {
            size_t fileSize_t = fileSize.QuadPart;
            result.content = (char*)VirtualAlloc(0, fileSize_t, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        
            if (result.content)
            {
                DWORD bytesRead;

                if (ReadFile(fileHandle, result.content, fileSize_t, &bytesRead, 0) && fileSize_t == bytesRead)
                {
                    result.size = fileSize_t;
                }
                else
                {
                    VirtualFree(result.content, 0, MEM_RELEASE);
                    result.content = NULL;
                }
            }
        }
    }

    result.name = filename;

    return result;
}

void freeFile(File *file)
{
    if (file->content)
    {
        VirtualFree(file, 0, MEM_RELEASE);
    }

    file->size = 0;
}

uint64_t getClockCounter(uint64_t frequency)
{
    LARGE_INTEGER queryResult;
    QueryPerformanceCounter(&queryResult);

    uint64_t value = queryResult.QuadPart;

    return value;
}

void debugString(std::string s)
{
    OutputDebugString(s.c_str());
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR args, int show)
{
	// flush stdout and stderr to console
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	
    const float targetFrameTime = 1.0f / 120.0f;
    INT desiredSchedulerMS = 1;
    timeBeginPeriod(desiredSchedulerMS);

    LARGE_INTEGER frequencyResult;
    QueryPerformanceFrequency(&frequencyResult);

    uint64_t performanceCountFrequency = frequencyResult.QuadPart;

	resizeOffscreenBuffer(&buffer, 600, 800);

    GameMemory memory;
    memory.temporaryMemorySize = 10 * 1024 * 1024;
    memory.permanentMemorySize = 100 * 1024 * 1024;
    memory.temporary = (char *)VirtualAlloc(0, memory.permanentMemorySize + memory.temporaryMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    memory.permanent = (char *)memory.temporary + memory.temporaryMemorySize;

    //memory.temporary = (char *)malloc(memory.temporaryMemorySize);
    //memory.permanent = (char *)malloc(memory.permanentMemorySize);

	HWND windowHandle = openWindow(instance, show);

    Input input[2] = { };
    Input *oldInput = &input[0];
    Input *newInput = &input[1];
	
    uint64_t lastCounter = getClockCounter(performanceCountFrequency);

	while (isRunning)
	{
        uint64_t loopStartCount = getClockCounter(performanceCountFrequency);
        *newInput = *oldInput;

		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{	
			if (message.message == WM_QUIT)
			{
				isRunning = false;
			}
			else if (message.message == WM_KEYDOWN || message.message == WM_KEYUP || message.message == WM_SYSKEYDOWN || message.message == WM_SYSKEYUP)
			{
				handleKeyStroke(newInput, message.wParam, message.lParam);
			}
			else
			{
				TranslateMessage(&message); 
				DispatchMessage(&message);
			}
		}

        POINT mousePosition;
        GetCursorPos(&mousePosition);
        ScreenToClient(windowHandle, &mousePosition);

        newInput->mouseX = mousePosition.x;
        newInput->mouseY = mousePosition.y;

        newInput->shootKey = (bool)(GetKeyState(VK_LBUTTON) & (1 << 15));
		
		VideoBuffer vBuffer;
		vBuffer.width = buffer.width;
		vBuffer.height = buffer.height;
		vBuffer.bytesPerPixel = buffer.bytesPerPixel;
		vBuffer.content = buffer.content;
		
		updateAndRender(&vBuffer, newInput, &memory);
		
		HDC deviceContext = GetDC(windowHandle);
		drawBuffer(deviceContext, &buffer);
		ReleaseDC(windowHandle, deviceContext);
	
        Input *tmp = oldInput;
        oldInput = newInput;
        newInput = tmp;

        uint64_t workCounter = getClockCounter(performanceCountFrequency);
        float secondsElapsed = (workCounter - lastCounter) / (float)performanceCountFrequency;
    
        if (secondsElapsed < targetFrameTime)
        {
            Sleep(1000.0f * (targetFrameTime - secondsElapsed));
        }
        else
        {
            debugString("Missed");
        } 

        uint64_t prevLastCounter = lastCounter;
        lastCounter = getClockCounter(performanceCountFrequency);

        float timePassed = (lastCounter - prevLastCounter) / (float)performanceCountFrequency;
        //debugString("Frametime: " + std::to_string(1.f / timePassed) + '\n');
    }
	
	return 0;
}
