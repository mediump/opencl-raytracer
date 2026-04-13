#pragma once

#include <CL/opencl.hpp>

struct Sphere
{
  cl_float  radius;
  cl_float  pad_1;
  cl_float  pad_2;
  cl_float  pad_3;
  cl_float3 position;
  cl_float3 color;
  cl_float3 emission;
};

void render_scene(int w, int h);
