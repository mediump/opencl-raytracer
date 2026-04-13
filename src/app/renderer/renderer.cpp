#include "renderer.h"

#include "app/state.h"
#include "utils/utils.h"

#define SPHERE_COUNT 9

#define float3(x, y, z) {{x, y, z}};

#define CLAMP_COLOR_01(v) v = \
  std::min(1.0F, std::max(0.0F, v))

std::vector<Sphere> init_spheres()
{
  std::vector<Sphere> cpu_spheres(SPHERE_COUNT);

  // left wall

  cpu_spheres[0].radius	= 200.0f;
  cpu_spheres[0].position = float3(-200.6f, 0.0f, 0.0f);
  cpu_spheres[0].color    = float3(0.75f, 0.25f, 0.25f);
  cpu_spheres[0].emission = float3(0.0f, 0.0f, 0.0f);

  // right wall

  cpu_spheres[1].radius	= 200.0f;
  cpu_spheres[1].position = float3(200.6f, 0.0f, 0.0f);
  cpu_spheres[1].color    = float3(0.25f, 0.75f, 0.25f);
  cpu_spheres[1].emission = float3(0.0f, 0.0f, 0.0f);

  // floor

  cpu_spheres[2].radius	= 200.0f;
  cpu_spheres[2].position = float3(0.0f, -200.4f, 0.0f);
  cpu_spheres[2].color	= float3(0.8f, 0.8f, 0.8f);
  cpu_spheres[2].emission = float3(0.0f, 0.0f, 0.0f);

  // ceiling

  cpu_spheres[3].radius	= 200.0f;
  cpu_spheres[3].position = float3(0.0f, 200.4f, 0.0f);
  cpu_spheres[3].color	= float3(0.8f, 0.8f, 0.8f);
  cpu_spheres[3].emission = float3(0.0f, 0.0f, 0.0f);

  // back wall

  cpu_spheres[4].radius   = 200.0f;
  cpu_spheres[4].position = float3(0.0f, 0.0f, -200.4f);
  cpu_spheres[4].color    = float3(0.9f, 0.8f, 0.7f);
  cpu_spheres[4].emission = float3(0.0f, 0.0f, 0.0f);

  // front wall

  cpu_spheres[5].radius   = 200.0f;
  cpu_spheres[5].position = float3(0.0f, 0.0f, 202.0f);
  cpu_spheres[5].color    = float3(0.9f, 0.8f, 0.7f);
  cpu_spheres[5].emission = float3(0.0f, 0.0f, 0.0f);

  // left sphere

  cpu_spheres[6].radius   = 0.16f;
  cpu_spheres[6].position = float3(-0.25f, -0.24f, -0.1f);
  cpu_spheres[6].color    = float3(0.9f, 0.9f, 0.9f);
  cpu_spheres[6].emission = float3(0.0f, 0.0f, 0.0f);

  // right sphere

  cpu_spheres[7].radius   = 0.16f;
  cpu_spheres[7].position = float3(0.25f, -0.24f, 0.1f);
  cpu_spheres[7].color    = float3(0.9f, 0.9f, 0.9f);
  cpu_spheres[7].emission = float3(0.0f, 0.0f, 0.0f);

  // light source

  cpu_spheres[8].radius   = 1.0f;
  cpu_spheres[8].position = float3(0.0f, 1.36f, 0.0f);
  cpu_spheres[8].color    = float3(0.0f, 0.0f, 0.0f);
  cpu_spheres[8].emission = float3(9.0f, 9.0f, 9.0f);

  return cpu_spheres;
}

void render_scene(const int w, const int h)
{
  // cl context

  LOG_MSG("Rendering at size: [%i, %i]", w, h);

  const OpenCLContext& ctx = State::get_cl_context();
  OpenCLProgram& prog = State::get_current_cl_program();

  const auto cpu_output = new cl_float3[w * h];
  const auto cl_output = cl::Buffer(ctx.context,
    CL_MEM_WRITE_ONLY, w * h * sizeof(cl_float3));

  // spheres

  std::vector<Sphere> spheres = init_spheres();

  // cl input buffers

  const auto cl_spheres = cl::Buffer(ctx.context,
    CL_MEM_READ_ONLY, SPHERE_COUNT * sizeof(Sphere));
  ctx.queue.enqueueWriteBuffer(cl_spheres, CL_TRUE,
    0, SPHERE_COUNT * sizeof(Sphere), spheres.data());

  // args

  prog.kernel.setArg(0, cl_spheres);
  prog.kernel.setArg(1, w);
  prog.kernel.setArg(2, h);
  prog.kernel.setArg(3, SPHERE_COUNT);
  prog.kernel.setArg(4, cl_output);

  std::size_t global_work_size = w * h;
  const std::size_t local_work_size =
    prog.kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(
      ctx.device);

  // ensure the global work size is a multiple of the local
  //  work size

  if (global_work_size % local_work_size != 0) {
    global_work_size = (global_work_size /
      local_work_size + 1) * local_work_size;
  }

  // launch kernel

  LOG_MSG(" > Rendering...");

  ctx.queue.enqueueNDRangeKernel(prog.kernel, NULL,
    global_work_size, local_work_size);
  ctx.wait_for_completion();

  LOG_MSG(" > Rendering complete");

  ctx.queue.enqueueReadBuffer(cl_output, CL_TRUE,
    0, w * h * sizeof(cl_float3), cpu_output);

  // send data back to window/viewport image

  const auto window = State::get_window();

  std::vector<unsigned char> data(w * h * 4);

  for (int i = 0; i < w * h; i++) {

    const int base = i * 4;

    const float r = CLAMP_COLOR_01(cpu_output[i].x);
    const float g = CLAMP_COLOR_01(cpu_output[i].y);
    const float b = CLAMP_COLOR_01(cpu_output[i].z);

    data[base + 0] = static_cast<unsigned char>(r * 255.0F);
    data[base + 1] = static_cast<unsigned char>(g * 255.0F);
    data[base + 2] = static_cast<unsigned char>(b * 255.0F);
    data[base + 3] = 255;

  }

  std::free(cpu_output);

  window->get_gui()->set_viewport_image_data(data.data());
}