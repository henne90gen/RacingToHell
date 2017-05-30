ifeq ($(OS), Windows_NT)
	PLATFORM_FILE := win32_RacingToHell.cpp
	PLATFORM_FLAGS := -mwindows
	EXTENSION := .exe
	CLEAN_CMD := del *$(EXTENSION)
	CL_FLAGS := -Od -MT -nologo -GR- -Gm- -EHsc- -Oi -FC -Zi -Iinclude
	CL_LINKER := -opt:ref -incremental:no user32.lib gdi32.lib winmm.lib
	GCC_LINKER := -lwinmm -luser32 -lgdi32 -lwinfreetype
	GCC_INCLUDES := -I./include -L./
else
	PLATFORM_FILE := linux_RacingToHell.cpp
	PLATFORM_FLAGS := -L/usr/X11R6/lib -lX11
	EXTENSION := .out
	CLEAN_CMD := rm -f *$(EXTENSION)
	GCC_LINKER := -lfreetype
	GCC_INCLUDES := -I./include
endif

GCC_FLAGS := -Wall -g -O0

SOURCE_FILES := Sound.cpp Math.cpp Renderer.cpp Font.cpp RacingToHell.cpp 

all:
	g++ $(GCC_FLAGS) $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) $(SOURCE_FILES) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

optimized:
	g++ -O3 $(PLATFORM_FLAGS) $(PLATFORM_FILE) $(SOURCE_FILES) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

cl:
	cl $(CL_FLAGS) freetype28.lib $(SOURCE_FILES) $(PLATFORM_FILE) /Febin\RacingToHell$(EXTENSION) -link $(CL_LINKER)
    
clean:
	$(CLEAN_CMD)
