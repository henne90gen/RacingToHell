cmake_minimum_required(VERSION 3.5)
project(RacingToHell)

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_NAME RacingToHell)


add_subdirectory(Source)

include_directories(${PROJECT_SOURCE_DIR}/Headers)


add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Add threading library
target_link_libraries(${EXECUTABLE_NAME} pthread)

# SFML library setup
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif ()
