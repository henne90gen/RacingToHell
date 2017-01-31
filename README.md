#RacingToHell

[![Build Status](https://travis-ci.org/henne90gen/RacingToHell.svg?branch=master)](https://travis-ci.org/henne90gen/RacingToHell) [![Build status](https://ci.appveyor.com/api/projects/status/y3sk6m09nwjqwjl7?svg=true)](https://ci.appveyor.com/project/henne90gen/racingtohell)

##Compiling the source code

###Linux

1.  Download and install CMake and all the required [libraries for SFML](http://www.sfml-dev.org/tutorials/2.4/compile-with-cmake.php#installing-dependencies)
1.  Clone the repository:
    -   `git clone https://github.com/henne90gen/RacingToHell`
1.  Create a directory called "build" and go into it:
    -   `mkdir build`
    -   `cd build`
1.  Generate the makefiles:
    -   `cmake -G "CodeBlocks - Unix Makefiles" ../`
1.  Execute the build:
    -   `cmake --build ./ --target RacingToHell -- -j 8`

Notes:
-   The compiled executable is located in `RacingToHell/build/bin`
-   This automatically downloads and builds SFML as well
-   Clion will automatically detect the relevant CMakeLists.txt and create a "cmake-build-debug" directory
    -   You can change the name of the default build directory under: `File -> Settings -> Build, Execution, Deployment -> CMake -> Generation path`

###Windows

1.  Download and install [MinGW-w64](https://sourceforge.net/projects/mingw-w64/)
    -   Add mingw64/bin to your path
1.  Clone the repository:
    -   `git clone https://github.com/henne90gen/RacingToHell`
1.  Create a directory called "build" and go into it:
    -   `mkdir build`
    -   `cd build`
1.  Generate the makefiles:
    -   `cmake -G "MinGW Makefiles" ../`
1.  Execute the build:
    -   `mingw32-make`

Notes:
-    Additionally you might have to set the following environment variables for cmake:
     -  `set CC=C:/PATH/TO/MINGW/mingw64/bin/gcc`
     -  `set CXX=C:/PATH/TO/MINGW/mingw64/bin/g++`
     -  `set CMAKE_C_COMPILER=C:/PATH/TO/MINGW/mingw64/bin/gcc`
     -  `set CMAKE_CXX_COMPILER=C:/PATH/TO/MINGW/mingw64/bin/g++`