#pragma once

#include "utils/image.h"

typedef void (*FGUICallback)(int, int);

struct GUIParams
{
  int image_width = 1280;
  int image_height = 720;
  Image viewport_image;

  float image_zoom = 1.0F;

  FGUICallback on_render;
};

class GUI
{
public:
  GUI();
  ~GUI();

  void init_gui();
  void draw_gui();

  void set_viewport_image_data(const unsigned char* data);

  [[nodiscard]]
  GUIParams& get_params();

private:
  GUIParams m_gui_params;
};