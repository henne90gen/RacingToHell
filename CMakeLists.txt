cmake_minimum_required(VERSION 3.16)

project(RacingToHell)

option(HOT_RELOAD "Build with code hot reloading enabled" OFF)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s WASM=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MIN_WEBGL_VERSION=2")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/res@/res")

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/html)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/html)

    configure_file(index.html ${CMAKE_BINARY_DIR}/html/index.html)
endif ()

# Third Party Dependencies
set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
set(BENCHMARK_INSTALL_DOCS OFF)
set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

include(FetchContent)
#FetchContent_Declare(
#        benchmark
#        GIT_REPOSITORY https://github.com/google/benchmark.git
#        GIT_TAG 0d98dba29d66e93259db7daa53a9327df767a415 # v1.6.1
#)
#FetchContent_Declare(
#        googletest
#        GIT_REPOSITORY https://github.com/google/googletest.git
#        GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
#)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50 # 9.1.0
)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e # 3.3.8
)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG 9aae45eb4a05a5a1f96be1ef37eb503a12ceb889 # v1.88
)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e # 0.9.9.8
)
FetchContent_Declare(
        freetype2
        GIT_REPOSITORY https://github.com/aseprite/freetype2.git
        GIT_TAG fbbcf50367403a6316a013b51690071198962920 # VER-2-10-0
)

FetchContent_MakeAvailable(
        fmt
        imgui
        glm
        freetype2
)

if (NOT ANDROID AND NOT EMSCRIPTEN)
    FetchContent_MakeAvailable(
            glfw
    )
    find_package(OpenGL REQUIRED)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED 20)

if (HOT_RELOAD)
    add_library(glad SHARED glad/src/glad.c)
    target_compile_definitions(glad PRIVATE GLAD_GLAPI_EXPORT GLAD_GLAPI_EXPORT_BUILD)
else ()
    add_library(glad STATIC glad/src/glad.c)
endif ()
target_include_directories(glad PUBLIC glad/include)

if (NOT ANDROID)
    add_library(imgui
            "${imgui_SOURCE_DIR}/imgui.cpp"
            "${imgui_SOURCE_DIR}/imgui_draw.cpp"
            "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
            "${imgui_SOURCE_DIR}/imgui_tables.cpp"
            "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
            "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
    target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
    target_link_libraries(imgui PUBLIC glfw)
else ()
    add_library(imgui
            "${imgui_SOURCE_DIR}/imgui.cpp"
            "${imgui_SOURCE_DIR}/imgui_draw.cpp"
            "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
            "${imgui_SOURCE_DIR}/imgui_tables.cpp"
            "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
            "${imgui_SOURCE_DIR}/backends/imgui_impl_android.cpp")
    target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
    target_link_libraries(imgui PUBLIC glad)
endif ()

function(register_resources resources_var)
    foreach (resource IN LISTS ARGN)
        string(MAKE_C_IDENTIFIER ${resource} identifier)
        get_filename_component(resource_absolute ${resource} ABSOLUTE)
        string(APPEND resource_definitions "Resource ${identifier}_resource(\"${resource}\");\n")
        string(APPEND resource_additions "    platform.resources[${identifier}_resource.resource_name] = &${identifier}_resource;\n")
    endforeach ()
    set(output_file ${CMAKE_CURRENT_BINARY_DIR}/resources.cpp)
    file(WRITE ${output_file}
            "#include \"Resources.h\"\n"
            "#include <string>\n\n"
            "${resource_definitions}\n"
            "#ifdef HOT_RELOAD\n"
            "extern \"C\"\n"
            "#if WIN32\n"
            "__declspec(dllexport)\n"
            "#endif\n"
            "#endif\n"
            "void init_resources(Platform &platform) {\n"
            "${resource_additions}"
            "}\n"
            )
    set(${resources_var} ${output_file} PARENT_SCOPE)
    message(STATUS "Registering resources done")
endfunction()

register_resources(RESOURCES
        res/icon.bmp
        res/shaders/fragment.glsl
        res/shaders/vertex.glsl
        res/font/Arial.ttf
        res/font/ComicSans.ttf
        res/font/DejaVuSansMono.ttf
        res/sound/music/level1.wav
        res/sound/gameOver.wav
        res/sound/shotAI.wav
        res/sound/shotPlayer.wav
        res/textures/bullet.bmp
        res/textures/canister.bmp
        res/textures/explosion.bmp
        res/textures/bosses/tank.bmp
        res/textures/bosses/tank_cannon.bmp
        res/textures/cars/cars.bmp
        res/textures/cars/player0.bmp
        res/textures/cars/player1.bmp
        res/textures/cars/player2.bmp
        res/textures/cars/player3.bmp
        res/textures/cars/player4.bmp
        res/textures/cars/player5.bmp
        res/textures/cars/traffic0.bmp
        res/textures/cars/traffic1.bmp
        res/textures/cars/traffic2.bmp
        res/textures/cars/traffic3.bmp
        res/textures/cars/traffic4.bmp
        res/textures/cars/traffic5.bmp
        res/textures/cars/traffic6.bmp
        res/textures/roads/road0.bmp
        res/textures/roads/road1.bmp
        res/textures/roads/road2.bmp
        res/textures/roads/road3.bmp
        res/textures/toolbox.bmp)

set(SOURCES
        src/Boss.cpp
        src/Collision.cpp
        src/Demos.cpp
        src/GameDemo.cpp
        src/GameMenu.cpp
        src/Helper.cpp
        src/Memory.cpp
        src/Init.cpp
        src/MyMath.cpp
        src/Player.cpp
        src/RacingToHell.cpp
        src/Renderer.cpp
        src/Resources.cpp
        src/Shader.cpp
        src/Sound.cpp
        ${RESOURCES})

if (HOT_RELOAD)
    add_library(game SHARED ${SOURCES})
    set_target_properties(game
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/temp"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/temp"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/temp"
            )
    target_compile_definitions(game PRIVATE GLAD_GLAPI_EXPORT)

    if (WIN32)
        target_link_libraries(game PUBLIC OpenGL::GL glad)
        add_custom_command(TARGET game POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:game> ${CMAKE_BINARY_DIR}/game.dll.new && ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/game.dll.new ${CMAKE_BINARY_DIR}/game.dll
                )
    else ()
        target_include_directories(game PRIVATE glad/include)
        add_custom_command(TARGET game POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:game> ${CMAKE_BINARY_DIR}/libgame.so.new && ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/libgame.so.new ${CMAKE_BINARY_DIR}/libgame.so
                )
    endif ()
    target_compile_definitions(game PUBLIC HOT_RELOAD)
else ()
    add_library(game STATIC ${SOURCES})
    target_include_directories(game PRIVATE glad/include)
endif ()

target_link_libraries(game PUBLIC
        fmt::fmt
        glm::glm
        freetype)
target_include_directories(game PUBLIC ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_property(TARGET game PROPERTY CXX_STANDARD 20)
set_property(TARGET game PROPERTY POSITION_INDEPENDENT_CODE ON)

if (NOT ANDROID)
    add_executable(RacingToHell src/main.cpp)
    target_link_libraries(RacingToHell
            fmt::fmt
            glm::glm
            freetype
            glfw
            glad)
    if (NOT EMSCRIPTEN)
        target_link_libraries(RacingToHell OpenGL::GL)
    endif ()
    set_property(TARGET RacingToHell PROPERTY CXX_STANDARD 20)
    if (HOT_RELOAD)
        target_compile_definitions(RacingToHell PUBLIC HOT_RELOAD GLAD_GLAPI_EXPORT)
        add_dependencies(RacingToHell game)
    else ()
        target_link_libraries(RacingToHell game)
    endif ()
endif ()
