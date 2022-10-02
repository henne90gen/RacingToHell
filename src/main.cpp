#include <glad/glad.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <chrono>
#include <filesystem>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "RacingToHell.h"
#include "Resources.h"

GLFWwindow *glfw_window = nullptr;

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

void startImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void finishImGuiFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Platform::abort(const std::string &msg) {
    spdlog::error(msg);
    ::exit(1);
}

void Platform::log(const std::string &msg) { spdlog::info(msg); }

static std::chrono::time_point<std::chrono::high_resolution_clock> program_start;
int64_t Platform::time() {
    auto now = std::chrono::high_resolution_clock::now();
    auto diff = program_start - now;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
}

File Platform::read_file(const std::string &file_path, bool is_resource) {
    auto final_file_path = file_path;
    if (is_resource) {
        final_file_path = memory.base_path + "/" + file_path;
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
    spdlog::info("Exiting...");
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

    spdlog::info("Updated the viewport to x={}, y={}, width={}, height={}", offsetX, offsetY, viewWidth, viewHeight);

    platform->memory.doResize = true;
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    auto platform = reinterpret_cast<Platform *>(glfwGetWindowUserPointer(window));
    resizeViewport(platform, width, height);
}

int main(int argc, char **argv) {
    spdlog::info("Starting RacingToHell");

    if (glfwInit() == 0) {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (glfw_window == nullptr) {
        spdlog::error("Failed to create window");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(glfw_window);
    auto procAddress = reinterpret_cast<GLADloadproc>(glfwGetProcAddress);
    auto status = gladLoadGLLoader(procAddress);
    if (status == 0) {
        spdlog::error("Failed to initialize GLAD");
        return 1;
    }

    const std::string &current_path = std::filesystem::current_path().string();
    std::string_view base_path = current_path;
    if (argc == 2) {
        base_path = std::string_view(argv[1]);
    }
    spdlog::info("Setting base path to '{}'", base_path);
    if (!std::filesystem::exists(std::string(base_path) + "/res")) {
        spdlog::error("Failed to find resources directory!");
        return 1;
    }

    Platform platform = {};
    platform.memory = initGameMemory(base_path);
    glfwSetWindowUserPointer(glfw_window, (void *)&platform);

    // set up callbacks
    glfwSetFramebufferSizeCallback(glfw_window, glfw_framebuffer_size_callback);

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
    spdlog::info("Graphics Card: {}", renderer);
    spdlog::info("OpenGL Version: {}", version);

    Input input[2] = {};
    Input *oldInput = &input[0];
    Input *newInput = &input[1];

    while (glfwWindowShouldClose(glfw_window) == 0) {
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

        startImGuiFrame();

        *newInput = *oldInput;
        platform.input = newInput;
        update_and_render(platform);

        Input *tmp = oldInput;
        oldInput = newInput;
        newInput = tmp;

        finishImGuiFrame();

        glfwSwapBuffers(glfw_window);
        glfwPollEvents();
    }

    spdlog::info("RacingToHell has been terminated");

    glfwTerminate();
    return 0;
}
