ifeq ($(OS), Windows_NT)
else

all: library
	g++ $(GCC_FLAGS) $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

library:
	g++ -shared -Wall -Werror -fpic src/RacingToHell.cpp -o librth.so.new
	mv -f librth.so.new librth.so

optimized:
	g++ -O3 $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) $(SOURCE_FILES) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

endif