#pragma once


#include "os/window.h"
#include "opencl/cl_context.h"
#include "utils/utils.h"

class State
{
  State()
    : m_window(std::make_shared<Window>())
  {
    LOG_MSG("App state initialized");
  }
  ~State() = default;

  State(const State&) = delete;
  State& operator=(const State&) = delete;

protected:
  static State* get()
  {
    static State instance;
    return &instance;
  }

public:
  static void init_cl()
  {
    const auto inst = get();
    inst->m_opencl_context = create_cl_context();
  }

  [[nodiscard]]
  static OpenCLContext& get_cl_context()
  {
    return get()->m_opencl_context;
  }

  static void set_current_cl_program(const char* kernel_path,
                              const char* entry_point)
  {
    const auto inst = get();
    inst->m_current_program = create_program(
      inst->m_opencl_context, kernel_path, entry_point);
  }

  [[nodiscard]]
  static OpenCLProgram& get_current_cl_program()
  {
    return get()->m_current_program;
  }

  static void init_app(const int width, const int height)
  {
    get()->m_window->create_window(width, height);
  }

  [[nodiscard]]
  static std::shared_ptr<Window> get_window()
  {
    return get()->m_window;
  }

private:
  std::shared_ptr<Window> m_window;

  OpenCLContext m_opencl_context;
  OpenCLProgram m_current_program;
};