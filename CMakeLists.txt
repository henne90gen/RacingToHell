cmake_minimum_required(VERSION 3.2)

project(RacingToHell)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_NAME RacingToHell)

add_subdirectory(SFML)
add_subdirectory(Source)

include_directories(${PROJECT_SOURCE_DIR}/Headers)
include_directories(./SFML/include)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(${EXECUTABLE_NAME} pthread)
