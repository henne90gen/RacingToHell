ifeq ($(OS), Windows_NT)

cl:
	windows\build
# TODO: CLEAN_CMD := 

else

PLATFORM_FILE := linux/linux_RacingToHell.cpp
PLATFORM_FLAGS := -L/usr/X11R6/lib
EXTENSION := .out
CLEAN_CMD := rm -f *$(EXTENSION)
GCC_LINKER := -ldl -lfreetype -lXxf86vm -lXext -lX11 -lasound -lGLEW -lGLU -lGL
GCC_INCLUDES := -Iinclude -Isrc
GCC_FLAGS := -Wall -g -O0 -march=athlon64 # not sure how to make this system independent

all: game_code
	g++ $(GCC_FLAGS) $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

game_code:
	g++ $(GCC_FLAGS) -shared -fPIC $(GCC_INCLUDES) src/RacingToHell.cpp -o librth.so.new -lfreetype
	mv -f librth.so.new librth.so

optimized: game_code
	g++ -O3 $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

endif

clean:
	$(CLEAN_CMD)
