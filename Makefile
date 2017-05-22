UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	PLATFORM_FILE := linux_RacingToHell.cpp
	PLATFORM_FLAGS :=
	EXTENSION := .out
else
	PLATFORM_FILE := win32_RacingToHell.cpp
	PLATFORM_FLAGS := -mwindows
	EXTENSION := .exe
endif

all:
	g++ -Wall $(PLATFORM_FLAGS) $(PLATFORM_FILE) -o RacingToHell$(EXTENSION)

clean:
	rm -f *$(EXTENSION) *.o
