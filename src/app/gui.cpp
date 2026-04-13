#include "gui.h"

#include "utils/utils.h"

#include <imgui.h>
#include <imgui_internal.h>

#define ZOOM_SPEED 0.1F
#define MIN_ZOOM 0.1F

void recreate_viewport_image(GUIParams* params)
{
  params->viewport_image = create_checkerboard_image(
    params->image_width,
    params->image_height);
}

void draw_settings_panel(GUIParams* params,
                         ImGuiID central_dock_id)
{
  bool settings_open;
  ImGui::Begin("Settings", &settings_open,
    ImGuiWindowFlags_NoCollapse);

  // image dimensions

  ImGui::Text("Image Dimensions");

  int w = params->image_width;
  int h = params->image_height;

  if (ImGui::InputInt("Width", &w)) {
    params->image_width = w;
    recreate_viewport_image(params);
  }

  if (ImGui::InputInt("Height", &h)) {
    params->image_height = h;
    recreate_viewport_image(params);
  }

  if (ImGui::Button("Render")) {
    params->on_render(params->image_width,
                      params->image_height);
  }

  ImGui::End();
}

void poll_zoom(GUIParams* params)
{
  params->image_zoom +=
    ImGui::GetIO().MouseWheel * ZOOM_SPEED;
  params->image_zoom = std::max(
    params->image_zoom, MIN_ZOOM);
}

void draw_viewport(GUIParams* params,
                   ImGuiID central_dock_id)
{
  bool image_open;
  ImGui::Begin("Viewport Image", &image_open,
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoScrollWithMouse);

  // calculate viewport zoom

  if (ImGui::IsWindowHovered()) {
    poll_zoom(params);
  }

  const ImVec2 zoomAdj = {
    params->image_zoom * params->viewport_image.width,
    params->image_zoom * params->viewport_image.height
  };

  // centering

  ImGui::SetCursorPos(
    ImVec2((ImGui::GetContentRegionAvail().x -
            zoomAdj.x) *
             0.5F,
           (ImGui::GetContentRegionAvail().y -
            zoomAdj.y) *
             0.5F));

  ImGui::Image(params->viewport_image.texture, zoomAdj);

  ImGui::End();
}

void GUI::init_gui()
{
  const Image viewport_image = create_checkerboard_image(
    m_gui_params.image_width,
    m_gui_params.image_height);

  m_gui_params.viewport_image = viewport_image;
}

void GUI::draw_gui()
{
  const ImGuiID central_dock_id =
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

  // Set up docking layout on first frame
  static bool first_frame = true;

  if (first_frame) {
    first_frame = false;

    ImGui::DockBuilderRemoveNode(central_dock_id);
    ImGui::DockBuilderAddNode(central_dock_id);

    ImGuiID dock_left, dock_right;
    ImGui::DockBuilderSplitNode(central_dock_id, ImGuiDir_Left, 0.7f, &dock_left, &dock_right);

    ImGui::DockBuilderDockWindow("Viewport Image", dock_left);
    ImGui::DockBuilderDockWindow("Settings", dock_right);

    ImGui::DockBuilderFinish(central_dock_id);
  }

  draw_settings_panel(&m_gui_params, central_dock_id);
  draw_viewport(&m_gui_params, central_dock_id);
}

GUIParams& GUI::get_params()
{
  return m_gui_params;
}

GUI::GUI()
{
  m_gui_params = {};
}

GUI::~GUI() = default;

void GUI::set_viewport_image_data(const unsigned char* data)
{
  m_gui_params.viewport_image = create_image(
    m_gui_params.image_width,
    m_gui_params.image_height,
    data);
}