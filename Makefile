ifeq ($(OS), Windows_NT)
	PLATFORM_FILE := windows/win32_RacingToHell.cpp
	PLATFORM_FLAGS := -mwindows
	EXTENSION := .exe
	CLEAN_CMD := del *$(EXTENSION)
	CL_FLAGS := -Od -MT -nologo -GR- -Gm- -EHsc- -Oi -FC -Zi -Iinclude -Isrc
	CL_LINKER := -opt:ref -incremental:no user32.lib gdi32.lib winmm.lib opengl32.lib
	GCC_LINKER := -lwinmm -luser32 -lgdi32 -lwinfreetype
	GCC_INCLUDES := -Iinclude -Isrc -Lwindows
	include windows/Makefile
else
	PLATFORM_FILE := linux/linux_RacingToHell.cpp
	PLATFORM_FLAGS := -L/usr/X11R6/lib
	EXTENSION := .out
	CLEAN_CMD := rm -f *$(EXTENSION)
	GCC_LINKER := -ldl -lfreetype -lX11 -lasound -lGL -lGLU -lGLEW
	GCC_INCLUDES := -Iinclude -Isrc -L.
	include linux/Makefile
endif

GCC_FLAGS := -Wall -g -O0

clean:
	$(CLEAN_CMD)
