# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

project(RacingToHellAndroid)

cmake_minimum_required(VERSION 3.18)

add_definitions(-DANDROID=1)
set(ANDROID ON)

add_subdirectory(../../ parent)

# find NDK logging library "log"
find_library(logging_library log)

add_library(native-lib SHARED src/main/cpp/android_RacingToHell.cpp)
target_include_directories(native-lib PRIVATE ../../src)
target_link_libraries(native-lib PRIVATE
        game
        ${log-lib}
        android
        freetype
        GLESv2)
