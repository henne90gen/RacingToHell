ifeq ($(OS), Windows_NT)
	PLATFORM_FILE := win/win32_RacingToHell.cpp
	PLATFORM_FLAGS := -mwindows
	EXTENSION := .exe
	CLEAN_CMD := del *$(EXTENSION)
	CL_FLAGS := -Od -MT -nologo -GR- -Gm- -EHsc- -Oi -FC -Zi -Iinclude
	CL_LINKER := -opt:ref -incremental:no user32.lib gdi32.lib winmm.lib
	GCC_LINKER := -lwinmm -luser32 -lgdi32 -lwinfreetype
	GCC_INCLUDES := -I./include -L./
else
	PLATFORM_FILE := linux/linux_RacingToHell.cpp
	PLATFORM_FLAGS := -L/usr/X11R6/lib
	EXTENSION := .out
	CLEAN_CMD := rm -f *$(EXTENSION)
	GCC_LINKER := -lfreetype -lX11 -lasound
	GCC_INCLUDES := -Iinclude -Isrc
endif

GCC_FLAGS := -Wall -g -O0

SOURCE_FILES := src/Memory.cpp src/Sound.cpp src/Math.cpp src/Renderer.cpp src/Font.cpp src/RacingToHell.cpp

include linux/Makefile windows/Makefile

clean:
	$(CLEAN_CMD)
