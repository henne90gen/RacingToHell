game_code:
	g++ $(GCC_FLAGS) -shared  -fpic -Iinclude src/RacingToHell.cpp -o librth.so.new
	mv -f librth.so.new librth.so

all: game_code
	g++ $(GCC_FLAGS) $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) -o RacingToHell$(EXTENSION) $(GCC_LINKER)

optimized: game_code
	g++ -O3 $(GCC_INCLUDES) $(PLATFORM_FLAGS) $(PLATFORM_FILE) -o RacingToHell$(EXTENSION) $(GCC_LINKER)
