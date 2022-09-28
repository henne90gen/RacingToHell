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

ABORT(abort) {
    spdlog::error(message);
    exit(1);
}

LOG(rth_log) { spdlog::info(message); }

/**
 * Returns the time in nanoseconds that has passed since the program start
 */
static std::chrono::time_point<std::chrono::high_resolution_clock> program_start;
QUERY_TIME(queryTime) {
    auto now = std::chrono::high_resolution_clock::now();
    auto diff = program_start - now;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();
}

/**
 * Reads an entire file into memory
 * This might be a very expensive operation, if the file is very large
 */
READ_FILE(readFile) {
    rth_log("Reading " + fileName);

    FILE *fileHandle = fopen(fileName.c_str(), "rb");
    if (!fileHandle) {
        abort("Could not open file " + fileName);
    }
    fseek(fileHandle, 0, SEEK_END);
    long int fileSize = ftell(fileHandle);
    rewind(fileHandle);

    char *content = (char *)malloc(fileSize + 1);
    fread(content, fileSize, 1, fileHandle);
    fclose(fileHandle);

    content[fileSize] = 0; // 0-terminated

    File file = {};
    file.size = fileSize;
    file.name = fileName;
    file.content = content;

    return file;
}

/**
 * Writes an entire file to disk
 */
WRITE_FILE(writeFile) {
    rth_log("Writing " + fileName);

    FILE *fHandle = fopen(fileName.c_str(), "wb");
    if (fHandle == NULL) {
        rth_log("ERROR - Failed to open file for writing");
        return false;
    }

    if (fwrite((void *)file->content, 1, file->size, fHandle) != file->size) {
        std::string errorMsg = "ERROR - Failed to write " + std::to_string(file->size) + " bytes to file";
        rth_log(errorMsg);
        return false;
    }

    fclose(fHandle);
    fHandle = NULL;
    return true;
}

/**
 * Releases the memory that was allocated for the given file
 */
FREE_FILE(freeFile) {
    if (file->content) {
        free(file->content);
    }

    file->size = 0;
}

/**
 * Ends the game gracefully
 */
EXIT_GAME(exitGame) {
    rth_log("Exiting");
    //    glfwSetWindowShouldClose();
    exit(0);
}

GameMemory initGameMemory(std::string_view base_path) {
    GameMemory memory = {};
    memory.doResize = true;
    memory.aspectRatio = 16.0f / 9.0f;

    memory.base_path = base_path;

    memory.abort = abort;
    memory.log = rth_log;
    memory.queryTime = queryTime;
    memory.readFile = readFile;
    memory.freeFile = freeFile;
    memory.exitGame = exitGame;

    memory.temporaryMemorySize = 10 * 1024 * 1024;
    memory.permanentMemorySize = 100 * 1024 * 1024;
    memory.temporary = (char *)malloc(memory.temporaryMemorySize);
    memory.permanent = (char *)malloc(memory.permanentMemorySize);

    return memory;
}

void resizeViewport(GameMemory *memory, int windowWidth, int windowHeight) {
    int viewWidth = windowHeight * memory->aspectRatio;
    int viewHeight = windowHeight;
    int offsetX = 0;
    int offsetY = 0;
    if (viewWidth > windowWidth) {
        viewWidth = windowWidth;
        viewHeight = windowWidth / memory->aspectRatio;
    } else {
        offsetX = (windowWidth - viewWidth) / 2;
    }
    if (windowHeight > viewHeight) {
        offsetY = (windowHeight - viewHeight) / 2;
    }

    glViewport(offsetX, offsetY, viewWidth, viewHeight);

    spdlog::info("Updated the viewport to x={}, y={}, width={}, height={}", offsetX, offsetY, viewWidth, viewHeight);

    memory->doResize = true;
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    auto memory = reinterpret_cast<GameMemory *>(glfwGetWindowUserPointer(window));
    resizeViewport(memory, width, height);
}

int main(int argc, char **argv) {
    program_start = std::chrono::high_resolution_clock::now();

    spdlog::info("Starting RacingToHell");
    GLFWwindow *window = nullptr;

    if (glfwInit() == 0) {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Failed to create window");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
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
    GameMemory memory = initGameMemory(base_path);
    glfwSetWindowUserPointer(window, (void *)&memory);

    // set up callbacks
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

    // to disable vsync uncomment this line
    //    glfwSwapInterval(0);

    // triggering it once "manually" to ensure the aspect ratio is set up
    // correctly
    resizeViewport(&memory, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    // ImGui installs its own glfw callbacks, which will then call our
    // previously installed callbacks
    initImGui(window);

    init_resources();

    glEnable(GL_DEPTH_TEST);

    //  enableOpenGLDebugging();
    std::string versionString = std::string((const char *)glGetString(GL_VERSION));
    spdlog::info("OpenGL Version: {}", versionString);
    // TODO query graphics hardware information

    Input input[2] = {};
    Input *oldInput = &input[0];
    Input *newInput = &input[1];

    while (glfwWindowShouldClose(window) == 0) {
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

        startImGuiFrame();

        *newInput = *oldInput;

        update_and_render(input, &memory);

        Input *tmp = oldInput;
        oldInput = newInput;
        newInput = tmp;

        finishImGuiFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    spdlog::info("RacingToHell has been terminated");

    glfwTerminate();
    return 0;
}
