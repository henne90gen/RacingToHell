ifeq ($(OS), Windows_NT)
	PLATFORM_FILE := win32_RacingToHell.cpp
	PLATFORM_FLAGS := -mwindows
	CL_FLAGS := -Od -MT -nologo -GR- -Gm- -EHa- -Oi -FC -Zi
	CL_LINKER := -opt:ref -incremental:no user32.lib gdi32.lib winmm.lib
	GCC_LINKER := -lwinmm -luser32 -lgdi32
	EXTENSION := .exe
	CLEAN_CMD := del *$(EXTENSION)
else
	PLATFORM_FILE := linux_RacingToHell.cpp
	PLATFORM_FLAGS := -L/usr/X11R6/lib -lX11
	EXTENSION := .out
	CLEAN_CMD := rm -f *$(EXTENSION)
	GCC_LINKER := -lfreetype
	GCC_INCLUDES := -I./include
endif

GCC_FLAGS := -Wall -g -O0

SOURCE_FILES := Font.cpp RacingToHell.cpp

all:
	g++ $(GCC_FLAGS) $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) $(SOURCE_FILES) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

optimized:
	g++ -O3 $(PLATFORM_FLAGS) $(PLATFORM_FILE) $(SOURCE_FILES) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

cl:
	cl $(CL_FLAGS) $(SOURCE_FILES) $(PLATFORM_FILE) -o bin/RacingToHell$(EXTENSION) -link $(CL_LINKER)
    
clean:
	$(CLEAN_CMD)
