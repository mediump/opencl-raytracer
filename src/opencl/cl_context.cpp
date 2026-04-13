#include "cl_context.h"
#include "utils/utils.h"

#include <iostream>
#include <vector>

void pick_platform(
  cl::Platform* platform,
  const std::vector<cl::Platform>& platformList)
{
  if (platformList.empty()) {
    LOG_MSG("No OpenCL platforms found");
    *platform = nullptr;
  } else {
    *platform = platformList[0];
  }
}

void pick_device(cl::Device* device,
                 const std::vector<cl::Device>& deviceList)
{
  if (deviceList.empty()) {
    LOG_MSG("No OpenCL devices found");
    *device = nullptr;
  } else {
    *device = deviceList[0];
  }
}

OpenCLContext create_cl_context()
{
  OpenCLContext context = {};

  LOG_MSG("Initializing OpenCL");

  std::vector<cl::Platform> platformList;
  cl::Platform::get(&platformList);

  // setup OpenCL context

  cl::Platform platform;
  pick_platform(&platform, platformList);

  LOG_MSG(" > Using OpenCL platform: %s",
          platform.getInfo<CL_PLATFORM_NAME>().c_str());

  std::vector<cl::Device> deviceList;
  platform.getDevices(CL_DEVICE_TYPE_ALL, &deviceList);

  cl::Device& device = context.device;
  pick_device(&device, deviceList);

  LOG_MSG("Using OpenCL device: %s",
          device.getInfo<CL_DEVICE_NAME>().c_str());
  LOG_MSG(" > Max compute units: %i",
          device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>());
  LOG_MSG(" > Max work group size: %llu",
          device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>());

  context.context = cl::Context(device);
  context.queue = cl::CommandQueue(context.context, device);

  LOG_MSG("OpenCL initialized");

  return context;
}

OpenCLProgram create_program(const OpenCLContext& context,
                             const char* source,
                             const char* entry_point)
{
  // compile kernel code

  const std::string code = load_text_file(source);
  const auto program = cl::Program(context.context, code);
  const cl_int result = program.build({context.device});

  if (result == CL_BUILD_PROGRAM_FAILURE) {
    FATAL(
      "Failed to build program: %s",
      program
        .getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.device)
        .c_str());
    EXIT(1);
  }
  if (result) {
    FATAL("Error during OpenCL code compilation. Code: %i",
          result);
    EXIT(1);
  }

  LOG_MSG(" > Built OpenCL kernel '%s'", source);

  // create kernel

  const cl::Kernel kernel(program, entry_point);

  // return struct
  OpenCLProgram outProgram = {};
  outProgram.program = program;
  outProgram.kernel = kernel;

  return outProgram;
}
