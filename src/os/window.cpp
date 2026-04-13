#include "window.h"

#include "app/gui.h"
#include "app/style.h"
#include "app/renderer/renderer.h"
#include "utils/utils.h"

#include <CL/opencl.hpp>
#include <iostream>

void platform_init(const char** platform,
                   const char** glsl_version)
{
  glfwInit();

  if (!glfwInit()) {
    FATAL("Unable to create GLFW window");
    EXIT(1);
  }

#if defined(IMGUI_IMPL_OPENGL_ES2)

  *platform = "WebGL 1.0";
  *glsl_version = "#version 100";

  GLFW_VERSION(2, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

#elif defined(IMGUI_IMPL_OPENGL_ES3)

  *platform = "WebGL 2.0";
  *glsl_version = "#version 300 es";

  GLFW_VERSION(3, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

#elif defined(__APPLE__)

  *platform = "OpenGL 3.2 (Apple)";
  *glsl_version = "#version 150";

  GLFW_VERSION(3, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#else

  *platform = "OpenGL 3.0";
  *glsl_version = "#version 130";

  GLFW_VERSION(3, 0);

#endif
}

void imgui_init(const Window* window,
                const char* glsl_version)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  init_style();

  ImGui_ImplGlfw_InitForOpenGL(window->get_handle(), true);

#ifdef __EMSCRIPTEN__

  // TODO: Emscripten callbacks

#endif

  ImGui_ImplOpenGL3_Init(glsl_version);

  LOG_MSG(" > Initializing ImGui with GLSL Version: ['%s']",
    glsl_version);
}

void imgui_update(const std::shared_ptr<GUI>& gui)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  gui->draw_gui();
}

void imgui_present()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_shutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void glad_init(const Window* window)
{
  glfwMakeContextCurrent(window->get_handle());
  glfwSwapInterval(1);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(
       glfwGetProcAddress))) {
    FATAL("Failed to initialize GLAD");
    EXIT(1);
  }
}

void window_init(Window* window, const char* glsl_version)
{
  glad_init(window);

  glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

  imgui_init(window, glsl_version);
  window->get_gui()->init_gui();
}

void window_destroy(const Window* window)
{
  imgui_shutdown();
  glfwDestroyWindow(window->get_handle());
  glfwTerminate();
}

void window_update(Window* window)
{
  glfwPollEvents();

  // clear
  glClear(GL_COLOR_BUFFER_BIT);

  // gui

  const std::shared_ptr<GUI> gui = window->get_gui();

  imgui_update(gui);
  imgui_present();

  glfwSwapBuffers(window->get_handle());
}

void on_render(const int w, const int h)
{
  render_scene(w, h);
}

void Window::create_window(const int width,
                           const int height)
{
  const char* platform;
  const char* glsl_version;
  platform_init(&platform, &glsl_version);

  LOG_MSG("Initializing window for: %s", platform);

  m_window_data.width = width;
  m_window_data.height = height;

  GUIParams& params = m_gui->get_params();
  params.on_render = [](const int w, const int h) {
    on_render(w, h);
  };

  char titleBuffer[100];
  snprintf(titleBuffer,
           sizeof(titleBuffer),
           "OpenCL Raytracer [%s, %s]",
           "CUDA",
           platform);

  const float main_scale =
    ImGui_ImplGlfw_GetContentScaleForMonitor(
      glfwGetPrimaryMonitor());

  const float w = static_cast<float>(width) * main_scale;
  const float h = static_cast<float>(height) * main_scale;

  m_window_data.handle =
    glfwCreateWindow(static_cast<int>(w),
                     static_cast<int>(h),
                     "Raytracer [OpenCL]",
                     nullptr,
                     nullptr);

  if (!get_handle()) {
    FATAL("Unable to create GLFW window");
    EXIT(1);
  }

  window_init(this, glsl_version);

  while (!glfwWindowShouldClose(get_handle())) {
    window_update(this);
  }

  window_destroy(this);
}

WindowData& Window::get_data()
{
  return m_window_data;
}

GLFWwindow* Window::get_handle() const
{
  return m_window_data.handle;
}

std::shared_ptr<GUI> Window::get_gui()
{
  return m_gui;
}

Window::Window()
  : m_gui(std::make_shared<GUI>())
{
  m_window_data = {};
  m_window_data.handle = nullptr;
}

Window::~Window() = default;