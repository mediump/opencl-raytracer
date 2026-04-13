#include "image.h"

#include <cstdlib>

#define CHECKER_SIZE 20
#define COLOR_0 0x00
#define COLOR_1 0xFF

Image create_image(const int width,
                   const int height,
                   const unsigned char* data)
{
  Image img = {};
  img.width = width;
  img.height = height;

  // opengl texture

  glGenTextures(1, &img.texture);
  glBindTexture(GL_TEXTURE_2D, img.texture);

  // filtering

  glTexParameteri(
    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(
    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // upload pixels to texture

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               width,
               height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               data);

  return img;
}

unsigned char* create_checkerboard_image_data(
  const int width,
  const int height,
  const int channels)
{
  const size_t image_size = width * height * channels;
  const auto image =
    static_cast<unsigned char*>(std::malloc(image_size));

  if (image == nullptr) {
    return nullptr;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      const int checker_x = x / CHECKER_SIZE;
      const int checker_y = y / CHECKER_SIZE;

      const unsigned char color =
        ((checker_x + checker_y) % 2) ? COLOR_0 : COLOR_1;

      const int pixel_index = (y * width + x) * channels;

      for (int c = 0; c < channels; c++) {
        image[pixel_index + c] = color;
      }
    }
  }

  return image;
}

Image create_checkerboard_image(const int width,
                                const int height)
{
  const unsigned char* data =
    create_checkerboard_image_data(width, height, 4);

  return create_image(width, height, data);
}