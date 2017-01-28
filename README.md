#RacingToHell

[![Build Status](https://travis-ci.org/henne90gen/RacingToHell.svg?branch=master)](https://travis-ci.org/henne90gen/RacingToHell)

##Setup to compile the source

- Download and install CMake and SFML-2.4.0
- Clone the repository.
- The CMakeLists.txt in the root directory contains everything necessary to run the build with CMake.
- A build command might look something like this: cmake --build /path/to/repo/RacingToHell/cmake-build-debug --target RacingToHell -- -j 8
- Make sure that SFML is either installed through the package manager, if you're on Linux or that it's on the PATH, if you're on Windows.
