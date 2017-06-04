#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <dsound.h>

#include "RacingToHell.h"
#include "win32_RacingToHell.h"

static bool isRunning = true;
static OffscreenBuffer buffer;
static LPDIRECTSOUNDBUFFER secondaryBuffer;

void debugString(std::string s)
{
    OutputDebugString(s.c_str());
}

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
        debugString("DBits failed.");
	}
}

typedef HRESULT _DirectSoundCreate_(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);

void initDSound(HWND window, uint32_t samplesPerSecond, int32_t bufferSize)
{
    //Bibliothek laden
    HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");

    if (DSoundLibrary)
    {
        //DirectSound Objekt 
        _DirectSoundCreate_ *directSoundCreate = (_DirectSoundCreate_ *)GetProcAddress(DSoundLibrary, "DirectSoundCreate");

        LPDIRECTSOUND directSound;
        if (directSoundCreate && SUCCEEDED(directSoundCreate(0, &directSound, 0)))
        {
            WAVEFORMATEX waveFormat = {};
            waveFormat.wFormatTag = WAVE_FORMAT_PCM;
            waveFormat.nChannels = 2;
            waveFormat.nSamplesPerSec = samplesPerSecond;
            waveFormat.wBitsPerSample = 16;
            waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
            waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
            waveFormat.cbSize = 0;

            if (SUCCEEDED(directSound->SetCooperativeLevel(window, DSSCL_PRIORITY)))
            {
                //Primärbuffer "erstellen"
                DSBUFFERDESC bufferDescription = {};

                bufferDescription.dwSize = sizeof(bufferDescription);
                bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

                LPDIRECTSOUNDBUFFER primaryBuffer;
                if (SUCCEEDED(directSound->CreateSoundBuffer(&bufferDescription, &primaryBuffer, 0)))
                {
                    if (SUCCEEDED(primaryBuffer->SetFormat(&waveFormat)))
                    {
                        debugString("Primaerbuffer wurde gesetzt\n");
                    }
                    else
                    {
                        abort("Primärbufferformat konnte nicht gesetzt werden.");
                    }
                }
                else
                {
                    abort("Primärbuffer konnte nicht erstellt werden.");
                }
            }
            else
            {
                abort("directSound->SetCooperativeLevel fehlgeschlagen.");
            }

            //Sekundärbuffer "erstellen"
            DSBUFFERDESC bufferDescription = {};

            bufferDescription.dwSize = sizeof(bufferDescription);
            bufferDescription.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
            bufferDescription.dwBufferBytes = bufferSize;
            bufferDescription.lpwfxFormat = &waveFormat;

            if (SUCCEEDED(directSound->CreateSoundBuffer(&bufferDescription, &secondaryBuffer, 0)))
            {
                debugString("Sekundärbuffer wurde gesetzt\n");
            }

            //Sound abspielen
        }
        else
        {
            abort("Soundbibliothek konnte nicht geladen werden.");
        }
    }
}

void fillSoundBuffer(SoundOutput *soundOutput, DWORD byteToLock, DWORD bytesToWrite, SoundOutputBuffer *source)
{
    VOID *region1, *region2;
    DWORD region1Size, region2Size;

    if (SUCCEEDED(secondaryBuffer->Lock(byteToLock, bytesToWrite, &region1, &region1Size, &region2, &region2Size, 0)))
    {
        DWORD region1SampleCount = region1Size / soundOutput->bytesPerSample;
        int16_t *destSample = (int16_t *)region1;
        int16_t *sourceSample = source->samples;

        for (DWORD sampleIndex = 0; sampleIndex < region1SampleCount; sampleIndex++)
        {
            *destSample++ = *sourceSample++;
            *destSample++ = *sourceSample++;

            ++soundOutput->runningSampleIndex;
        }

        destSample = (int16_t *)region2;
        DWORD region2SampleCount = region2Size / soundOutput->bytesPerSample;

        for (DWORD sampleIndex = 0; sampleIndex < region2SampleCount; sampleIndex++)
        {
            *destSample++ = *sourceSample++;
            *destSample++ = *sourceSample++;

            ++soundOutput->runningSampleIndex;
        }

        secondaryBuffer->Unlock(region1, region1Size, region2, region2Size);
    }
}

void clearSoundBuffer(SoundOutput *soundOutput)
{
    VOID *region1;
    DWORD region1Size;
    VOID *region2;
    DWORD region2Size;

    if (SUCCEEDED(secondaryBuffer->Lock(0, soundOutput->secondaryBufferSize, &region1, &region1Size, &region2, &region2Size, 0)))
    {
        uint8_t *destSample = (uint8_t *)region1;
        for (DWORD byteIndex = 0; byteIndex < region1Size; byteIndex++)
        {
            *destSample++ = 0;
        }

        destSample = (uint8_t *)region2;
        for (DWORD byteIndex = 0; byteIndex < region2Size; byteIndex++)
        {
            *destSample++ = 0;
        }

        secondaryBuffer->Unlock(region1, region1Size, region2, region2Size);
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

HWND openWindow(HINSTANCE instance, int show, unsigned width, unsigned height)
{
    WNDCLASS windowClass = {};
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hIcon = LoadIcon(0, IDI_WINLOGO);
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "Window Class";

    RegisterClass(&windowClass);

    RECT windowRect = {0 ,0, (LONG)width, (LONG)height};
    AdjustWindowRectEx(&windowRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false, 0);

    HWND windowHandle = CreateWindowEx(
        0,
        windowClass.lpszClassName,
        WINDOW_TITLE,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        instance,
        NULL
        );

    if(!windowHandle) {
    	abort("Could not create window.");
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

uint64_t getClockCounter()
{
    LARGE_INTEGER queryResult;
    QueryPerformanceCounter(&queryResult);

    uint64_t value = queryResult.QuadPart;

    return value;
}

void sleep(float time, uint64_t frequency)
{
    float timePassed = 0.0f;

    uint64_t startClock = getClockCounter();

    while (timePassed < time)
    {
        timePassed = (getClockCounter() - startClock) / (float)(frequency);
    }
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR args, int show)
{
	// flush stdout and stderr to console
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	
    const float targetFrameTime = 1.0f / 60.f;
    INT desiredSchedulerMS = 1;
    timeBeginPeriod(desiredSchedulerMS);

    LARGE_INTEGER frequencyResult;
    QueryPerformanceFrequency(&frequencyResult);

    uint64_t performanceCountFrequency = frequencyResult.QuadPart;

	resizeOffscreenBuffer(&buffer, WINDOW_WIDTH, WINDOW_HEIGHT);

    GameMemory memory;
    memory.temporaryMemorySize = 10 * 1024 * 1024;
    memory.permanentMemorySize = 300 * 1024 * 1024;
    memory.temporary = (char *)VirtualAlloc(0, memory.permanentMemorySize + memory.temporaryMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    memory.permanent = (char *)memory.temporary + memory.temporaryMemorySize;

	HWND windowHandle = openWindow(instance, show, WINDOW_WIDTH, WINDOW_HEIGHT);

    SoundOutput soundOutput;
    soundOutput.safetyBytes = (soundOutput.samplesPerSecond * soundOutput.bytesPerSample) * targetFrameTime;

    initDSound(windowHandle, soundOutput.samplesPerSecond, soundOutput.secondaryBufferSize);
    clearSoundBuffer(&soundOutput);
    secondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

    int16_t *samples = (int16_t *)VirtualAlloc(0, soundOutput.secondaryBufferSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    bool soundIsValid = false;
    DWORD audioLatencyBytes = 0;
    float audioLatencySeconds = 0.0f;

    Input input[2] = { };
    Input *oldInput = &input[0];
    Input *newInput = &input[1];
	
    uint64_t flipWallClock = getClockCounter();
    uint64_t lastCounter = getClockCounter();

    bool LButtonWasPressed = false;
    bool RButtonWasPressed = false;

	while (isRunning)
	{
        uint64_t loopStartCount = getClockCounter();
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

        bool LButtonPressed = (bool)(GetKeyState(VK_LBUTTON) & (1 << 15));
        bool RButtonPressed = (bool)(GetKeyState(VK_RBUTTON) & (1 << 15));

        newInput->mousePosition = { (float)mousePosition.x, (float)mousePosition.y };
        newInput->shootKeyPressed = LButtonPressed || RButtonPressed;
        newInput->shootKeyClicked = (LButtonPressed && !LButtonWasPressed) || (RButtonPressed && !RButtonWasPressed);
		
        LButtonWasPressed = LButtonPressed;
        RButtonWasPressed = RButtonPressed;

		VideoBuffer vBuffer;
		vBuffer.width = buffer.width;
		vBuffer.height = buffer.height;
		vBuffer.bytesPerPixel = buffer.bytesPerPixel;
		vBuffer.content = buffer.content;
		
		updateAndRender(&vBuffer, newInput, &memory);
		
        uint64_t audioWallClock = getClockCounter();
        float fromBeginToAudioSeconds = (audioWallClock - flipWallClock) / (float)performanceCountFrequency;

        DWORD playCursor, writeCursor;

        if (secondaryBuffer->GetCurrentPosition(&playCursor, &writeCursor) == DS_OK)
        {
            DWORD byteToLock = 0, targetCursor = 0, bytesToWrite = 0;

            if (!soundIsValid)
            {
                soundOutput.runningSampleIndex = writeCursor / soundOutput.bytesPerSample;
                soundIsValid = true;
            }

            byteToLock = (soundOutput.runningSampleIndex * soundOutput.bytesPerSample) % soundOutput.secondaryBufferSize;

            DWORD expectedSoundBytesPerFrame = (soundOutput.samplesPerSecond * soundOutput.bytesPerSample) * targetFrameTime;

            float secondsLeftUntilFlip = (targetFrameTime - fromBeginToAudioSeconds);
            DWORD expectedBytesUntilFlip = (DWORD)(secondsLeftUntilFlip / (float)targetFrameTime * (float)expectedSoundBytesPerFrame);

            DWORD expectedFrameBoundaryByte = playCursor + expectedBytesUntilFlip;

            DWORD safeWriteCursor = writeCursor;

            if (safeWriteCursor < playCursor)
                safeWriteCursor += soundOutput.secondaryBufferSize;

            if (safeWriteCursor < playCursor)
            {
                abort("safeWriteCursor < playCursor");
            }

            safeWriteCursor += soundOutput.safetyBytes;

            bool latentAudio = safeWriteCursor >= expectedFrameBoundaryByte;

            if (latentAudio)
            {
                targetCursor = writeCursor + expectedSoundBytesPerFrame + soundOutput.safetyBytes;
            }
            else
            {
                targetCursor = (expectedFrameBoundaryByte + expectedSoundBytesPerFrame);
            }

            targetCursor %= soundOutput.secondaryBufferSize;

            if (byteToLock > targetCursor)
            {
                bytesToWrite = (soundOutput.secondaryBufferSize - byteToLock) + targetCursor;
            }
            else
            {
                bytesToWrite = targetCursor - byteToLock;
            }

            SoundOutputBuffer soundBuffer = {};
            soundBuffer.samplesPerSecond = soundOutput.samplesPerSecond;
            soundBuffer.sampleCount = bytesToWrite / soundOutput.bytesPerSample;
            soundBuffer.samples = samples;

            Sound::getSoundSamples(&memory, &soundBuffer);

            /*debug_marker *marker = &debugTimeMarker[debugMarkerIndex];
            marker->outputPlayCursor = playCursor;
            marker->outputWriteCursor = writeCursor;
            marker->outputLocation = byteToLock;
            marker->outputByteCount = bytesToWrite;
            marker->expectedFlipCursor = expectedFrameBoundaryByte; */

            audioLatencyBytes = 0;
            if (writeCursor > playCursor)
            {
                audioLatencyBytes = writeCursor - playCursor;
            }
            else
            {
                audioLatencyBytes = writeCursor + (soundOutput.secondaryBufferSize - playCursor);
            }

            audioLatencySeconds = ((float)audioLatencyBytes / (float)soundOutput.bytesPerSample) / (float)soundOutput.samplesPerSecond;

            /*	char buffer[256];
            sprintf(buffer, "BTL %d TC %d BTW %d PC %d WC %d DELTA = %d\n", byteToLock, targetCursor, bytesToWrite, playCursor, writeCursor, audioLatencyBytes);
            OutputDebugStringA(buffer); */

            fillSoundBuffer(&soundOutput, byteToLock, bytesToWrite, &soundBuffer);
        }
        else
        {
            soundIsValid = false;
        }

        uint64_t workCounter = getClockCounter();
        float secondsElapsed = (workCounter - lastCounter) / (float)performanceCountFrequency;

        if (secondsElapsed < targetFrameTime)
        {
            sleep(targetFrameTime - secondsElapsed, performanceCountFrequency);
        }
        else
        {
            debugString("Missed :");
        }

        uint64_t prevLastCounter = lastCounter;
        lastCounter = getClockCounter();

		HDC deviceContext = GetDC(windowHandle);
		drawBuffer(deviceContext, &buffer);
		ReleaseDC(windowHandle, deviceContext);
	
        Input *tmp = oldInput;
        oldInput = newInput;
        newInput = tmp;

        flipWallClock = getClockCounter();

        float timePassed = (lastCounter - prevLastCounter) / (float)performanceCountFrequency;
        debugString("Frametime: " + std::to_string(1.f / timePassed) + '\n');
    }
	
	return 0;
}
