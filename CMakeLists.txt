cmake_minimum_required(VERSION 3.2)

project(RacingToHell)

set(CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")

set(EXECUTABLE_NAME RacingToHell)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CLONE_SFML git clone https://github.com/SFML/SFML)
execute_process(COMMAND ${CLONE_SFML}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(SFML)
add_subdirectory(Source)

include_directories(${PROJECT_SOURCE_DIR}/Headers)
include_directories(./SFML/include)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(${EXECUTABLE_NAME} pthread)
