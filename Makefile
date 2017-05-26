ifeq ($(OS), Windows_NT)
	PLATFORM_FILE := win32_RacingToHell.cpp
	PLATFORM_FLAGS := -mwindows
    CL_FLAGS := -Od -MT -nologo -GR- -Gm- -EHa- -Oi -FC -Zi
    CL_LINKER := -opt:ref -incremental:no user32.lib gdi32.lib winmm.lib
	EXTENSION := .exe
	CLEAN_CMD := del *$(EXTENSION)
else
	PLATFORM_FILE := linux_RacingToHell.cpp
	PLATFORM_FLAGS := -L/usr/X11R6/lib -lX11
	EXTENSION := .out
	CLEAN_CMD := rm -f *$(EXTENSION)
endif

GCC_FLAGS := -Wall -g -O0

all:
	g++ $(GCC_FLAGS) $(PLATFORM_FLAGS) $(PLATFORM_FILE) RacingToHell.cpp -o RacingToHell$(EXTENSION)

cl:
	cl $(CL_FLAGS) RacingToHell.cpp $(PLATFORM_FILE) -o bin/RacingToHell$(EXTENSION) -link $(CL_LINKER)
    
clean:
	$(CLEAN_CMD)
