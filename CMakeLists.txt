cmake_minimum_required(VERSION 3.20)

project(RacingToHell)

# Third Party Dependencies
set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
set(BENCHMARK_INSTALL_DOCS OFF)
set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG 76fb40d95455f249bd70824ecfcae7a8f0930fa3 # v1.10.0
)
FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG 0d98dba29d66e93259db7daa53a9327df767a415 # v1.6.1
)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
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
        # benchmark
        # googletest
        spdlog
        glfw
        imgui
        glm
        freetype2
)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_library(imgui
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
target_link_libraries(imgui PUBLIC glad glfw)

add_library(glad src/glad/src/glad.c)
target_include_directories(glad PUBLIC src/glad/include)

add_executable(resource_generator tools/resource_generator.cpp)
target_link_libraries(resource_generator spdlog)

function(register_resource resource_list resource)
    string(MAKE_C_IDENTIFIER ${resource} identifier)
    get_filename_component(resource_absolute ${resource} ABSOLUTE)
    set(filename ${identifier}.cpp)
    add_custom_command(
            OUTPUT ${filename}
            COMMAND resource_generator ${resource_absolute} ${filename} ${resource} ${identifier}
            DEPENDS ${resource}
    )
    list(APPEND ${resource_list} ${filename})
    set(${resource_list} ${${resource_list}} PARENT_SCOPE)
endfunction()

register_resource(RESOURCES res/shaders/fragment.glsl)
register_resource(RESOURCES res/shaders/vertex.glsl)
register_resource(RESOURCES res/font/Arial.ttf)
register_resource(RESOURCES res/font/ComicSans.ttf)
register_resource(RESOURCES res/font/DejaVuSansMono.ttf)
register_resource(RESOURCES res/sound/music/level1.wav)
register_resource(RESOURCES res/sound/gameOver.wav)
register_resource(RESOURCES res/sound/shotAI.wav)
register_resource(RESOURCES res/sound/shotPlayer.wav)
register_resource(RESOURCES res/textures/bosses/tank.bmp)
register_resource(RESOURCES res/textures/bosses/tank_cannon.bmp)
register_resource(RESOURCES res/textures/cars/cars.bmp)
register_resource(RESOURCES res/textures/cars/player0.bmp)
register_resource(RESOURCES res/textures/cars/player1.bmp)
register_resource(RESOURCES res/textures/cars/player2.bmp)
register_resource(RESOURCES res/textures/cars/player3.bmp)
register_resource(RESOURCES res/textures/cars/player4.bmp)
register_resource(RESOURCES res/textures/cars/player5.bmp)
register_resource(RESOURCES res/textures/cars/traffic0.bmp)
register_resource(RESOURCES res/textures/cars/traffic1.bmp)
register_resource(RESOURCES res/textures/cars/traffic2.bmp)
register_resource(RESOURCES res/textures/cars/traffic3.bmp)
register_resource(RESOURCES res/textures/cars/traffic4.bmp)
register_resource(RESOURCES res/textures/cars/traffic5.bmp)
register_resource(RESOURCES res/textures/cars/traffic6.bmp)
register_resource(RESOURCES res/textures/roads/road0.bmp)
register_resource(RESOURCES res/textures/roads/road1.bmp)
register_resource(RESOURCES res/textures/roads/road2.bmp)
register_resource(RESOURCES res/textures/roads/road3.bmp)

add_library(game
        src/Boss.cpp
        src/Collision.cpp
        src/Demos.cpp
        src/GameDemo.cpp
        src/GameMenu.cpp
        src/Helper.cpp
        src/Memory.cpp
        src/Init.cpp
        src/MyMath.cpp
        src/RacingToHell.cpp
        src/Renderer.cpp
        src/Resources.cpp
        src/Shader.cpp
        src/Sound.cpp
        ${RESOURCES})
target_link_libraries(game PUBLIC
        glad
        spdlog
        glm::glm
        imgui
        freetype
        ${OPENGL_gl_LIBRARY})
target_include_directories(game PUBLIC ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_property(TARGET game PROPERTY CXX_STANDARD 20)

add_executable(RacingToHell src/main.cpp)
target_link_libraries(RacingToHell game glfw)
set_property(TARGET RacingToHell PROPERTY CXX_STANDARD 20)
