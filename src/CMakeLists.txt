add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

add_library(game
        Boss.cpp
        Collision.cpp
        Demos.cpp
        GameDemo.cpp
        GameMenu.cpp
        Helper.cpp
        Memory.cpp
        Init.cpp
        MyMath.cpp
        RacingToHell.cpp
        Renderer.cpp
        Shader.cpp
        Sound.cpp)
target_link_libraries(game PUBLIC
        glad
        spdlog
        glm::glm
        imgui
        freetype
        ${OPENGL_gl_LIBRARY})
#target_include_directories(game PUBLIC ${OPENGL_INCLUDE_DIR})
#target_link_libraries(game ${OPENGL_gl_LIBRARY})

add_executable(RacingToHell main.cpp)
target_link_libraries(RacingToHell
        game
        glfw
        )
