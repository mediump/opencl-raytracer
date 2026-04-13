#pragma once

#include "utils/utils.h"

#include <CL/opencl.hpp>

struct OpenCLContext
{
  cl::CommandQueue queue;
  cl::Device device;
  cl::Context context;

  void wait_for_completion() const
  {
    const cl_int err = queue.finish();
    if (err != CL_SUCCESS) {
      FATAL("OpenCL error when running queue. Code: '%i'",
            err);
    }
  }
};

struct OpenCLProgram
{
  cl::Program program;
  cl::Kernel kernel;
};

[[nodiscard]] OpenCLContext create_cl_context();

[[nodiscard]] OpenCLProgram create_program(
  const OpenCLContext& context,
  const char* source,
  const char* entry_point);
