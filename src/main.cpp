#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_demo.cpp>
#include <spdlog/spdlog.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

void initImGui(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
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

int main() {
  spdlog::info("Starting RacingToHell");
  GLFWwindow *window = nullptr;

  if (glfwInit() == 0) {
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(300, 500, "RacingToHell", nullptr, nullptr);
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

  // to disable vsync uncomment this line
  //    glfwSwapInterval(0);

  // glfwSetWindowUserPointer(window, scene);

  // triggering it once "manually" to ensure the aspect ratio is set up
  // correctly
  //  scene->onWindowResize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
  //  installCallbacks(window);

  // ImGui installs its own glfw callbacks, which will then call our previously
  // installed callbacks
  initImGui(window);

  glEnable(GL_DEPTH_TEST);

  //  enableOpenGLDebugging();

  while (glfwWindowShouldClose(window) == 0) {
    glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

    startImGuiFrame();

    ShowDemoWindowWidgets();

    finishImGuiFrame();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  spdlog::info("RacingToHell has been terminated");

  glfwTerminate();
  return 0;
}
