ifeq ($(OS), Windows_NT)
	PLATFORM_FILE := win32_RacingToHell.cpp
	PLATFORM_FLAGS := -mwindows
	EXTENSION := .exe
	CLEAN_CMD := del *$(EXTENSION)
else
	PLATFORM_FILE := linux_RacingToHell.cpp
	PLATFORM_FLAGS :=
	EXTENSION := .out
	CLEAN_CMD := rm -f *$(EXTENSION)
endif

GCC_FLAGS := -Wall -g -O0

all:
	g++ $(GCC_FLAGS) $(PLATFORM_FLAGS) $(PLATFORM_FILE) RacingToHell.cpp -o RacingToHell$(EXTENSION)

clean:
	$(CLEAN_CMD)
