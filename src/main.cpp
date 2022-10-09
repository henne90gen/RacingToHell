#include <glad/glad.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <chrono>
#include <filesystem>
#include <fmt/core.h>
#include <imgui.h>
#include <iostream>

#if EMSCRIPTEN
#include <emscripten.h>
#endif

#include "RacingToHell.h"
#include "Resources.h"

GLFWwindow *glfw_window = nullptr;
Platform platform = {};
Input input[2] = {};

void initImGui(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad
    // Controls

    //    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void start_im_gui_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void finish_im_gui_frame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Platform::abort(const std::string &msg) {
    std::cerr << msg << std::endl;
    ::exit(1);
}

void Platform::log(const std::string &msg) { std::cout << msg << std::endl; }

File Platform::read_file(const std::string &file_path, bool is_resource) {
    auto final_file_path = file_path;
    if (is_resource) {
#if EMSCRIPTEN
        final_file_path = "/" + file_path;
#else
        final_file_path = memory.base_path + "/" + file_path;
#endif
    }
    log("Reading " + final_file_path);

    FILE *fileHandle = fopen(final_file_path.c_str(), "rb");
    if (!fileHandle) {
        abort("Could not open file " + final_file_path);
    }
    fseek(fileHandle, 0, SEEK_END);
    long int file_size = ftell(fileHandle);
    rewind(fileHandle);

    char *content = (char *)std::malloc(file_size);
    fread(content, file_size, 1, fileHandle);
    fclose(fileHandle);

    File file = {};
    file.size = file_size;
    file.name = file_path;
    file.is_resource = is_resource;
    file.content = content;

    return file;
}

void Platform::free_file(File &file) {
    if (file.content) {
        std::free(file.content);
    }

    file.size = 0;
}

int64_t Platform::last_modified(const std::string &file_path) {
#if EMSCRIPTEN
    return 0;
#else
    auto last_write_time = std::filesystem::last_write_time(file_path);
    return std::chrono::time_point_cast<std::chrono::milliseconds>(last_write_time).time_since_epoch().count();
#endif
}

bool Platform::write_file(const File &file) {
    log("Writing " + file.name);

    FILE *fHandle = fopen(file.name.c_str(), "wb");
    if (fHandle == NULL) {
        log("ERROR - Failed to open file for writing");
        return false;
    }

    if (fwrite((void *)file.content, 1, file.size, fHandle) != file.size) {
        std::string errorMsg = "ERROR - Failed to write " + std::to_string(file.size) + " bytes to file";
        log(errorMsg);
        return false;
    }

    fclose(fHandle);
    fHandle = nullptr;
    return true;
}

void Platform::exit() {
    log("Exiting...");
    glfwSetWindowShouldClose(glfw_window, 1);
}

GameMemory initGameMemory(std::string_view base_path) {
    GameMemory memory = {};
    memory.doResize = true;
    memory.aspectRatio = 16.0f / 9.0f;

    memory.base_path = base_path;
    memory.temporaryMemorySize = 10 * 1024 * 1024;
    memory.permanentMemorySize = 100 * 1024 * 1024;
    memory.temporary = (char *)malloc(memory.temporaryMemorySize);
    memory.permanent = (char *)malloc(memory.permanentMemorySize);

    return memory;
}

void resizeViewport(Platform *platform, int windowWidth, int windowHeight) {
    int viewWidth = windowHeight * platform->memory.aspectRatio;
    int viewHeight = windowHeight;
    int offsetX = 0;
    int offsetY = 0;
    if (viewWidth > windowWidth) {
        viewWidth = windowWidth;
        viewHeight = windowWidth / platform->memory.aspectRatio;
    } else {
        offsetX = (windowWidth - viewWidth) / 2;
    }
    if (windowHeight > viewHeight) {
        offsetY = (windowHeight - viewHeight) / 2;
    }

    glViewport(offsetX, offsetY, viewWidth, viewHeight);

    std::cout << fmt::format("Updated the viewport to x={}, y={}, width={}, height={}", offsetX, offsetY, viewWidth,
                             viewHeight)
              << std::endl;

    platform->memory.doResize = true;
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    auto platform = reinterpret_cast<Platform *>(glfwGetWindowUserPointer(window));
    resizeViewport(platform, width, height);
}

/**
 * All *Clicked variables are filled here.
 * *Clicked signals the moment where we go from not pressing to pressing a button.
 */
void check_input_for_clicks(Input *input) {
    static bool up, down, left, right, enter, escape, plus, minus, shoot;

    input->upKeyClicked = input->upKeyPressed && !up;
    up = input->upKeyPressed;

    input->downKeyClicked = input->downKeyPressed && !down;
    down = input->downKeyPressed;

    input->leftKeyClicked = input->leftKeyPressed && !left;
    left = input->leftKeyPressed;

    input->rightKeyClicked = input->rightKeyPressed && !right;
    right = input->rightKeyPressed;

    input->enterKeyClicked = input->enterKeyPressed && !enter;
    enter = input->enterKeyPressed;

    input->escapeKeyClicked = input->escapeKeyPressed && !escape;
    escape = input->escapeKeyPressed;

    input->plusKeyClicked = input->plusKeyPressed && !plus;
    plus = input->plusKeyPressed;

    input->minusKeyClicked = input->minusKeyPressed && !minus;
    minus = input->minusKeyPressed;

    input->shootKeyClicked = input->shootKeyPressed && !shoot;
    shoot = input->shootKeyPressed;
}

// key – The [keyboard key](keys) that was pressed or released.
// scancode – The system-specific scancode of the key.
// action – `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`. Future releases may add more actions.
// mods – Bit field describing which [modifier keys](mods) were held down.
void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    platform.input->enterKeyPressed = key == GLFW_KEY_ENTER && action == GLFW_PRESS;
    platform.input->escapeKeyPressed = key == GLFW_KEY_ESCAPE && action == GLFW_PRESS;
    platform.input->upKeyPressed = key == GLFW_KEY_UP && action == GLFW_PRESS;
    platform.input->downKeyPressed = key == GLFW_KEY_DOWN && action == GLFW_PRESS;
    platform.input->leftKeyPressed = key == GLFW_KEY_LEFT && action == GLFW_PRESS;
    platform.input->rightKeyPressed = key == GLFW_KEY_RIGHT && action == GLFW_PRESS;
    platform.input->plusKeyPressed = key == GLFW_KEY_KP_ADD && action == GLFW_PRESS;
    platform.input->minusKeyPressed = key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS;
}

// window The window that received the event.
// button The [mouse button](@ref buttons) that was pressed or released.
// action One of `GLFW_PRESS` or `GLFW_RELEASE`. Future releases may add more actions.
// mods Bit field describing which [modifier keys](@ref mods) were held down.
void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    platform.input->shootKeyPressed = button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS;
}

// window The window that received the event.
// xpos The new cursor x-coordinate, relative to the left edge of the content area.
// ypos The new cursor y-coordinate, relative to the top edge of the content area.
void glfw_cursor_pos_callback(GLFWwindow *window, double x_pos, double y_pos) {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);

    auto mousePos = glm::vec2(x_pos, y_pos);
    mousePos /= glm::vec2(width, height);
    mousePos.y = 1.0F - mousePos.y;
    mousePos *= 2.0F;
    mousePos -= 1.0F;
    mousePos *= glm::vec2(float(width) / float(height), 1);
    platform.input->mousePosition = mousePos;
}

void run_main_loop() {
    static Input *oldInput = &input[0];
    static Input *newInput = &input[1];

    glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

    start_im_gui_frame();

    *newInput = *oldInput;
    platform.input = newInput;

    check_input_for_clicks(platform.input);

    update_and_render(platform);

    Input *tmp = oldInput;
    oldInput = newInput;
    newInput = tmp;

    finish_im_gui_frame();

    glfwSwapBuffers(glfw_window);
    glfwPollEvents();
}

int main(int argc, char **argv) {
    std::cout << "Starting RacingToHell" << std::endl;

    if (glfwInit() == 0) {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (glfw_window == nullptr) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(glfw_window);
    auto procAddress = reinterpret_cast<GLADloadproc>(glfwGetProcAddress);
    auto status = gladLoadGLLoader(procAddress);
    if (status == 0) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    const std::string &current_path = std::filesystem::current_path().string();
    std::string_view base_path = current_path;
    if (argc == 2) {
        base_path = std::string_view(argv[1]);
    }

    std::cout << fmt::format("Setting base path to '{}'", base_path) << std::endl;
#if !EMSCRIPTEN
    if (!std::filesystem::exists(std::string(base_path) + "/res")) {
        std::cerr << "Failed to find resources directory!" << std::endl;
        return 1;
    }
#endif

    platform.memory = initGameMemory(base_path);
    glfwSetWindowUserPointer(glfw_window, (void *)&platform);

    // set up callbacks
    glfwSetFramebufferSizeCallback(glfw_window, glfw_framebuffer_size_callback);
    glfwSetKeyCallback(glfw_window, glfw_key_callback);
    glfwSetMouseButtonCallback(glfw_window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(glfw_window, glfw_cursor_pos_callback);

    // to disable vsync uncomment this line
    //    glfwSwapInterval(0);

    // triggering it once "manually" to ensure the aspect ratio is set up
    // correctly
    resizeViewport(&platform, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    // ImGui installs its own glfw callbacks, which will then call our
    // previously installed callbacks
    initImGui(glfw_window);

    init_resources();

    glEnable(GL_DEPTH_TEST);

    //  enableOpenGLDebugging();

    std::string renderer = std::string((const char *)glGetString(GL_RENDERER));
    std::string version = std::string((const char *)glGetString(GL_VERSION));
    std::cout << fmt::format("Graphics Card: {}", renderer) << std::endl;
    std::cout << fmt::format("OpenGL Version: {}", version) << std::endl;

#if EMSCRIPTEN
    emscripten_set_main_loop(run_main_loop, 0, 1);
#else
    while (glfwWindowShouldClose(glfw_window) == 0) {
        run_main_loop();
    }
#endif

    std::cout << "RacingToHell has been terminated" << std::endl;

    glfwTerminate();
    return 0;
}
