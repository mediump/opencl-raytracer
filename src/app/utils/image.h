#pragma once

#include <glad/glad.h>

struct Image
{
  GLuint texture;
  int width;
  int height;
};

Image create_image(int width,
                   int height,
                   const unsigned char* data);

Image create_checkerboard_image(int width, int height);

