ifeq ($(OS), Windows_NT)
cl:
	windows\build
else

PLATFORM_FILE := linux/linux_RacingToHell.cpp
PLATFORM_FLAGS := -L/usr/X11R6/lib
EXTENSION := .out
CLEAN_CMD := rm -f *$(EXTENSION)
GCC_LINKER := -ldl -lfreetype -lXxf86vm -lXext -lX11 -lasound -lGLEW -lGLU -lGL
GCC_INCLUDES := -Iinclude -Isrc -L.
GCC_FLAGS := -Wall -g -O0 -mavx2

game_code:
	g++ $(GCC_FLAGS) -shared  -fpic -Iinclude src/RacingToHell.cpp -o librth.so.new
	mv -f librth.so.new librth.so

all: game_code
	g++ $(GCC_FLAGS) $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

optimized: game_code
	g++ -O3 $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

endif

clean:
	$(CLEAN_CMD)
