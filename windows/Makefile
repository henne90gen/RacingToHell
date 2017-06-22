all:
	g++ $(GCC_FLAGS) $(GCC_INCLUDES) -mwindows $(PLATFORM_FILE) $(SOURCE_FILES) -o RacingToHell${EXTENSION} $(GCC_LINKER)

optimized:
	g++ -O3 -mwindows $(PLATFORM_FILE) $(SOURCE_FILES) -o RacingToHell${EXTENSION} $(GCC_LINKER)

cl:
	windows\build
    
#cl $(CL_FLAGS) freetype28.lib $(SOURCE_FILES) $(PLATFORM_FILE) /Febin\RacingToHell${EXTENSION} -link $(CL_LINKER)