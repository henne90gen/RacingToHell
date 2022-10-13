#ifdef HOT_RELOAD
#if WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#endif

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

#include "Helper.h"
#include "RacingToHell.h"
#include "Resources.h"

#ifdef HOT_RELOAD
struct GameCode {
    std::string sourceDllPath;
    std::string tmpDllPath;
    int64_t lastModified = -1;

#if WIN32
    HMODULE gameCodeDLL = nullptr;
#else
    void *libraryHandle;
#endif

    update_and_render_func *update_and_render = nullptr;
    init_resources_func *init_resources = nullptr;

    bool load();
};
GameCode gameCode = {};
#endif

GLFWwindow *glfw_window = nullptr;
Platform platform = {};
auto previousTime = std::chrono::high_resolution_clock::now();

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

void platform_exit() {
    platform.log("Exiting...");
    glfwSetWindowShouldClose(glfw_window, 1);
}

void platform_abort(const std::string &msg) {
    std::cerr << msg << std::endl;
    ::exit(1);
}

void platform_log(const std::string &msg) { std::cout << msg << std::endl; }

File platform_read_file(const std::string &file_path, bool is_resource) {
    auto final_file_path = file_path;
    if (is_resource) {
#if EMSCRIPTEN
        final_file_path = "/" + file_path;
#else
        final_file_path = platform.memory.base_path + "/" + file_path;
#endif
    }
    platform.log("Reading " + final_file_path);

    FILE *fileHandle = fopen(final_file_path.c_str(), "rb");
    if (!fileHandle) {
        platform.abort("Could not open file " + final_file_path);
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

void platform_free_file(File &file) {
    if (file.content) {
        std::free(file.content);
    }

    file.size = 0;
}

int64_t platform_last_modified(const std::string &file_path) {
#if EMSCRIPTEN
    return 0;
#else
    std::error_code ec;
    auto last_write_time = std::filesystem::last_write_time(file_path, ec);
    if (ec) {
        return 0;
    }
    return std::chrono::time_point_cast<std::chrono::milliseconds>(last_write_time).time_since_epoch().count();
#endif
}

bool platform_write_file(const File &file) {
    platform.log("Writing " + file.name);

    FILE *fHandle = fopen(file.name.c_str(), "wb");
    if (fHandle == NULL) {
        platform.log("ERROR - Failed to open file for writing");
        return false;
    }

    if (fwrite((void *)file.content, 1, file.size, fHandle) != file.size) {
        std::string errorMsg = "ERROR - Failed to write " + std::to_string(file.size) + " bytes to file";
        platform.log(errorMsg);
        return false;
    }

    fclose(fHandle);
    fHandle = nullptr;
    return true;
}

GameMemory initGameMemory(std::string_view base_path) {
    GameMemory memory = {};
    memory.doResize = true;
    memory.aspectRatio = 9.0f / 16.0f;

    memory.base_path = base_path;
    memory.temporaryMemorySize = 10 * 1024 * 1024;
    memory.permanentMemorySize = 100 * 1024 * 1024;
    memory.temporary = (char *)malloc(memory.temporaryMemorySize);
    memory.permanent = (char *)malloc(memory.permanentMemorySize);

    return memory;
}

void load_window_icon() {
    std::string resource_name = "res/icon.bmp";
    auto file = platform.read_file(resource_name, true);
    if (file.content[0] != 'B' || file.content[1] != 'M') {
        platform.abort(resource_name + " is not a bitmap file.");
    }

    int fileHeaderSize = 14;
    BitmapHeader header = *((BitmapHeader *)(file.content + fileHeaderSize));

    if (header.bitsPerPixel != 32) {
        platform.abort("Image must have 32-bit of color depth.");
    }

    int width = static_cast<int>(header.width);
    int height = static_cast<int>(header.height);
    auto bytesPerPixel = header.bitsPerPixel / 8;
    void *content = malloc(width * height * bytesPerPixel);

    auto src = (void *)(file.content + header.size + fileHeaderSize);
    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            int srcIndex = (header.width - y - 1) * header.width + x;
            int destIndex = y * width + x;
            uint32_t color = ((uint32_t *)(src))[srcIndex];
            uint8_t red = (color & 0xff000000) >> 24;
            uint8_t green = (color & 0x00ff0000) >> 16;
            uint8_t blue = (color & 0x0000ff00) >> 8;
            uint8_t alpha = color & 0x000000ff;
            color = (alpha << 24) + (blue << 16) + (green << 8) + red;
            ((uint32_t *)(content))[destIndex] = color;
        }
    }

    GLFWimage icon = {};
    icon.width = width;
    icon.height = height;
    icon.pixels = static_cast<unsigned char *>(content);
    glfwSetWindowIcon(glfw_window, 1, &icon);

    free(content);
}

void resizeViewport(int windowWidth, int windowHeight) {
    int viewWidth = windowHeight * platform.memory.aspectRatio;
    int viewHeight = windowHeight;
    int offsetX = 0;
    int offsetY = 0;
    if (viewHeight > windowHeight) {
        viewHeight = windowHeight;
        viewWidth = windowHeight / platform.memory.aspectRatio;
    } else {
        offsetY = (windowHeight - viewHeight) / 2;
    }
    if (windowWidth > viewWidth) {
        offsetX = (windowWidth - viewWidth) / 2;
    }

    // TODO having windowWidth smaller than the aspect ratio is currently broken
    glViewport(offsetX, offsetY, viewWidth, viewHeight);

    platform.logf("Updated the viewport to x={}, y={}, width={}, height={}", offsetX, offsetY, viewWidth, viewHeight);

    platform.memory.doResize = true;
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) { resizeViewport(width, height); }

/**
 * All *Clicked variables are filled here.
 * *Clicked signals the moment where we go from not pressing to pressing a button.
 */
void check_input_for_clicks(Input &input) {
    static bool up, down, left, right, enter, escape, plus, minus, shoot;

    input.upKeyClicked = input.upKeyPressed && !up;
    up = input.upKeyPressed;

    input.downKeyClicked = input.downKeyPressed && !down;
    down = input.downKeyPressed;

    input.leftKeyClicked = input.leftKeyPressed && !left;
    left = input.leftKeyPressed;

    input.rightKeyClicked = input.rightKeyPressed && !right;
    right = input.rightKeyPressed;

    input.enterKeyClicked = input.enterKeyPressed && !enter;
    enter = input.enterKeyPressed;

    input.escapeKeyClicked = input.escapeKeyPressed && !escape;
    escape = input.escapeKeyPressed;

    input.plusKeyClicked = input.plusKeyPressed && !plus;
    plus = input.plusKeyPressed;

    input.minusKeyClicked = input.minusKeyPressed && !minus;
    minus = input.minusKeyPressed;

    input.shootKeyClicked = input.shootKeyPressed && !shoot;
    shoot = input.shootKeyPressed;
}

// window The window that received the event.
// key – The [keyboard key](keys) that was pressed or released.
// scancode – The system-specific scancode of the key.
// action – `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`. Future releases may add more actions.
// mods – Bit field describing which [modifier keys](mods) were held down.
void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ENTER) {
        platform.input.enterKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    if (key == GLFW_KEY_ESCAPE) {
        platform.input.escapeKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
        platform.input.upKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
        platform.input.downKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) {
        platform.input.leftKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) {
        platform.input.rightKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    if (key == GLFW_KEY_KP_ADD) {
        platform.input.plusKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    if (key == GLFW_KEY_KP_SUBTRACT) {
        platform.input.minusKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
}

// window The window that received the event.
// button The [mouse button](@ref buttons) that was pressed or released.
// action One of `GLFW_PRESS` or `GLFW_RELEASE`. Future releases may add more actions.
// mods Bit field describing which [modifier keys](@ref mods) were held down.
void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_1) {
        platform.input.shootKeyPressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    }
}

// window The window that received the event.
// xpos The new cursor x-coordinate, relative to the left edge of the content area.
// ypos The new cursor y-coordinate, relative to the top edge of the content area.
void glfw_cursor_pos_callback(GLFWwindow *window, double x_pos, double y_pos) {
    int windowWidth = 0;
    int windowHeight = 0;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    int viewWidth = windowHeight * platform.memory.aspectRatio;
    int viewHeight = windowHeight;
    int offsetX = 0;
    int offsetY = 0;
    if (viewHeight > windowHeight) {
        viewHeight = windowHeight;
        viewWidth = windowHeight / platform.memory.aspectRatio;
    } else {
        offsetY = (windowHeight - viewHeight) / 2;
    }
    if (windowWidth > viewWidth) {
        offsetX = (windowWidth - viewWidth) / 2;
    }

    auto mousePos = glm::vec2(x_pos, y_pos);
    mousePos -= glm::vec2(offsetX, offsetY);
    mousePos /= glm::vec2(viewWidth, viewHeight);
    mousePos.y = 1.0F - mousePos.y;
    mousePos *= 2.0F;
    mousePos -= 1.0F;
    mousePos *= glm::vec2(1, float(viewHeight) / float(viewWidth));
    platform.input.mousePosition = mousePos;
}

void setup_callbacks() {
    glfwSetFramebufferSizeCallback(glfw_window, glfw_framebuffer_size_callback);
    glfwSetKeyCallback(glfw_window, glfw_key_callback);
    glfwSetMouseButtonCallback(glfw_window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(glfw_window, glfw_cursor_pos_callback);
}

#ifdef HOT_RELOAD
#if WIN32
std::string GetLastErrorAsString() {
    // Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return {}; // No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    // Ask Win32 to give us the string version of that message ID.
    // The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet
    // know how long the message string will be).
    size_t size =
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    // Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    // Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}

bool GameCode::load() {
    if (gameCodeDLL) {
        FreeLibrary(gameCodeDLL);
        gameCodeDLL = nullptr;
    }
    isValid = false;
    update_and_render = nullptr;
    init_resources = nullptr;

    if (sourceDllPath.empty()) {
        char filename[1000];
        GetModuleFileNameA(nullptr, filename, sizeof(filename));
        auto exePath = std::filesystem::path(std::string(filename)).parent_path().generic_string();
        sourceDllPath = exePath + "/game.dll";
        tmpDllPath = exePath + "/game-tmp.dll";
    }

    lastModified = platform_last_modified(sourceDllPath);

    CopyFile(sourceDllPath.c_str(), tmpDllPath.c_str(), FALSE);
    gameCodeDLL = LoadLibraryA(tmpDllPath.c_str());

    if (gameCodeDLL) {
        update_and_render = (update_and_render_func *)GetProcAddress(gameCodeDLL, "update_and_render");
        if (!update_and_render) {
            platform_log(GetLastErrorAsString());
        }

        init_resources = (init_resources_func *)GetProcAddress(gameCodeDLL, "init_resources");
        if (!init_resources) {
            platform_log(GetLastErrorAsString());
        }

        isValid = update_and_render != nullptr && init_resources != nullptr;
    } else {
        platform_log(GetLastErrorAsString());
    }

    if (!isValid) {
        update_and_render = nullptr;
        init_resources = nullptr;
    }

    return !isValid;
}
#else
bool GameCode::load() {
    platform_log("Loading game code");
    if (libraryHandle != nullptr) {
        dlclose(libraryHandle);
        libraryHandle = nullptr;
    }

    update_and_render = nullptr;
    init_resources = nullptr;

    sourceDllPath = "./libgame.so";

    if (!std::filesystem::exists(sourceDllPath)) {
        return true;
    }

    lastModified = platform_last_modified(sourceDllPath);
    libraryHandle = dlopen(sourceDllPath.c_str(), RTLD_NOW);
    if (!libraryHandle) {
        platform_log("Couldn't load library: " + std::string(dlerror()));
        return true;
    }

    update_and_render = (update_and_render_func *)dlsym(libraryHandle, "update_and_render");
    if (!update_and_render) {
        platform_log("Couldn't load 'update_and_render' function: " + std::string(dlerror()));
        return true;
    }

    init_resources = (init_resources_func *)dlsym(libraryHandle, "init_resources");
    if (!init_resources) {
        platform_log("Couldn't load 'init_resources' function: " + std::string(dlerror()));
        return true;
    }

    return update_and_render == nullptr || init_resources == nullptr;
}
#endif
#endif

inline void update_and_render_game() {
#ifndef HOT_RELOAD
    update_and_render(platform);
#else
    auto last_modified = platform_last_modified(gameCode.sourceDllPath);
    if (last_modified > gameCode.lastModified) {
        if (gameCode.load()) {
            platform_abort("Failed to load game code");
        }
        gameCode.init_resources(&platform);
    }
    gameCode.update_and_render(&platform);
#endif
}

void run_main_loop() {
    auto now = std::chrono::high_resolution_clock::now();
    auto timeDiff = now - previousTime;
    platform.frameTimeMs = double(timeDiff.count()) / 1000000.0;

    glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

    start_im_gui_frame();

    check_input_for_clicks(platform.input);
    update_and_render_game();

    finish_im_gui_frame();

    glfwSwapBuffers(glfw_window);
    glfwPollEvents();

    previousTime = now;
}

int main(int argc, char **argv) {
    std::cout << "Starting RacingToHell" << std::endl;

#ifdef HOT_RELOAD
    if (gameCode.load()) {
        platform_abort("Failed to load game code");
    }
#endif

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
    platform.exit = platform_exit;
    platform.abort = platform_abort;
    platform.log = platform_log;
    platform.last_modified = platform_last_modified;
    platform.read_file = platform_read_file;
    platform.free_file = platform_free_file;

#ifdef HOT_RELOAD
    gameCode.init_resources(&platform);
#else
    init_resources();
#endif

    load_window_icon();

    setup_callbacks();

    // to disable vsync uncomment this line
    //    glfwSwapInterval(0);

    // triggering it once "manually" to ensure the aspect ratio is set up correctly
    resizeViewport(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    // ImGui installs its own glfw callbacks, which will then call our previously installed callbacks
    initImGui(glfw_window);

    glEnable(GL_DEPTH_TEST);
    //  enableOpenGLDebugging();

    std::string renderer = std::string((const char *)glGetString(GL_RENDERER));
    std::string version = std::string((const char *)glGetString(GL_VERSION));
    platform.logf("Graphics Card: {}", renderer);
    platform.logf("OpenGL Version: {}", version);

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
