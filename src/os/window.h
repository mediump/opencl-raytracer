#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/gui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <memory>

typedef void (*FWindowCallback)();

class GLFWwindow;

struct WindowData
{
  GLFWwindow* handle;
  int width;
  int height;
  FWindowCallback init, destroy, update;
};

class Window
{
public:
  Window();
  ~Window();

  void create_window(int width, int height);

  [[nodiscard]]
  WindowData& get_data();

  [[nodiscard]]
  GLFWwindow* get_handle() const;

  [[nodiscard]]
  std::shared_ptr<GUI> get_gui();

private:
  WindowData m_window_data{};
  std::shared_ptr<GUI> m_gui;
};

