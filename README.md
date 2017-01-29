#RacingToHell

[![Build Status](https://travis-ci.org/henne90gen/RacingToHell.svg?branch=master)](https://travis-ci.org/henne90gen/RacingToHell)

##Compiling the source code

###Linux

1. Download and install CMake and all the required [libraries for SFML](http://www.sfml-dev.org/tutorials/2.4/compile-with-cmake.php#installing-dependencies)
1. Clone the repository:
    - `git clone https://github.com/henne90gen/RacingToHell`
1. Create a directory called "build" and go into it:
    - `mkdir build`
    - `cd build`
1. Generate the makefiles:
    - `cmake -G "CodeBlocks - Unix Makefiles" ../`
1. Execute the build:
    - `cmake --build ./ --target RacingToHell -- -j 8`

Notes:
- This automatically downloads and builds SFML as well
- Clion will automatically detect the relevant CMakeLists.txt and create a "cmake-build-debug" directory
    - You can change the name of the default build directory under: `File -> Settings -> Build, Execution, Deployment -> CMake -> Generation path`

###Windows
