#pragma once

#include <imgui.h>

inline void init_style()
{
  ImGuiStyle* style = &ImGui::GetStyle();
  style->GrabRounding = 0.0F;
  style->FrameRounding = 0.0F;
  style->TabRounding = 0.0F;

  ImVec4* colors = style->Colors;

  colors[ImGuiCol_Text] =
    ImVec4(1.00F, 1.00F, 1.00F, 1.00F);
  colors[ImGuiCol_TextDisabled] =
    ImVec4(0.33F, 0.33F, 0.33F, 1.00F);
  colors[ImGuiCol_WindowBg] =
    ImVec4(0.18F, 0.18F, 0.18F, 1.00F);
  colors[ImGuiCol_ChildBg] =
    ImVec4(0.03F, 0.03F, 0.03F, 1.00F);
  colors[ImGuiCol_PopupBg] =
    ImVec4(0.23F, 0.23F, 0.23F, 1.00F);
  colors[ImGuiCol_Border] =
    ImVec4(0.14F, 0.14F, 0.14F, 0.99F);
  colors[ImGuiCol_BorderShadow] =
    ImVec4(0.00F, 0.00F, 0.00F, 0.00F);
  colors[ImGuiCol_FrameBg] =
    ImVec4(0.03F, 0.03F, 0.03F, 1.00F);
  colors[ImGuiCol_FrameBgHovered] =
    ImVec4(0.03F, 0.03F, 0.03F, 1.00F);
  colors[ImGuiCol_FrameBgActive] =
    ImVec4(0.03F, 0.03F, 0.03F, 1.00F);
  colors[ImGuiCol_TitleBg] =
    ImVec4(0.18F, 0.18F, 0.18F, 1.00F);
  colors[ImGuiCol_TitleBgActive] =
    ImVec4(0.23F, 0.23F, 0.23F, 1.00F);
  colors[ImGuiCol_TitleBgCollapsed] =
    ImVec4(0.18F, 0.18F, 0.18F, 1.00F);
  colors[ImGuiCol_MenuBarBg] =
    ImVec4(0.18F, 0.18F, 0.18F, 1.00F);
  colors[ImGuiCol_ScrollbarBg] =
    ImVec4(0.02F, 0.02F, 0.02F, 0.53F);
  colors[ImGuiCol_ScrollbarGrab] =
    ImVec4(0.07F, 0.07F, 0.07F, 1.00F);
  colors[ImGuiCol_ScrollbarGrabHovered] =
    ImVec4(0.18F, 0.17F, 0.17F, 1.00F);
  colors[ImGuiCol_ScrollbarGrabActive] =
    ImVec4(0.18F, 0.18F, 0.18F, 1.00F);
  colors[ImGuiCol_CheckMark] =
    ImVec4(0.30F, 0.60F, 0.10F, 1.00F);
  colors[ImGuiCol_SliderGrab] =
    ImVec4(0.30F, 0.60F, 0.10F, 1.00F);
  colors[ImGuiCol_SliderGrabActive] =
    ImVec4(0.43F, 0.90F, 0.11F, 1.00F);
  colors[ImGuiCol_Button] =
    ImVec4(0.03F, 0.03F, 0.03F, 1.00F);
  colors[ImGuiCol_ButtonHovered] =
    ImVec4(0.38F, 0.51F, 0.51F, 0.80F);
  colors[ImGuiCol_ButtonActive] =
    ImVec4(0.54F, 0.55F, 0.55F, 1.00F);
  colors[ImGuiCol_Header] =
    ImVec4(0.04F, 0.04F, 0.04F, 1.00F);
  colors[ImGuiCol_HeaderHovered] =
    ImVec4(0.38F, 0.51F, 0.51F, 0.80F);
  colors[ImGuiCol_HeaderActive] =
    ImVec4(0.03F, 0.03F, 0.03F, 1.00F);
  colors[ImGuiCol_Separator] =
    ImVec4(0.16F, 0.16F, 0.16F, 0.50F);
  colors[ImGuiCol_SeparatorHovered] =
    ImVec4(0.10F, 0.40F, 0.75F, 0.78F);
  colors[ImGuiCol_SeparatorActive] =
    ImVec4(0.10F, 0.40F, 0.75F, 1.00F);
  colors[ImGuiCol_ResizeGrip] =
    ImVec4(0.58F, 0.58F, 0.58F, 1.0F);
  colors[ImGuiCol_ResizeGripHovered] =
    ImVec4(0.58F, 0.58F, 0.58F, 1.0F);
  colors[ImGuiCol_ResizeGripActive] =
    ImVec4(0.58F, 0.58F, 0.58F, 1.0F);
  colors[ImGuiCol_TabHovered] =
    ImVec4(0.23F, 0.23F, 0.24F, 0.80F);
  colors[ImGuiCol_Tab] =
    ImVec4(0.02F, 0.02F, 0.02F, 1.00F);
  colors[ImGuiCol_TabSelected] =
    ImVec4(0.02F, 0.02F, 0.02F, 1.00F);
  colors[ImGuiCol_TabSelectedOverline] =
    ImVec4(0.58F, 0.58F, 0.58F, 1.0F);
  colors[ImGuiCol_TabDimmed] =
    ImVec4(0.02F, 0.02F, 0.02F, 1.00F);
  colors[ImGuiCol_TabDimmedSelected] =
    ImVec4(0.02F, 0.02F, 0.02F, 1.00F);
  colors[ImGuiCol_TabDimmedSelectedOverline] =
    ImVec4(0.58F, 0.58F, 0.58F, 1.0F);
  colors[ImGuiCol_DockingPreview] =
    ImVec4(0.58F, 0.58F, 0.58F, 1.0F);
  colors[ImGuiCol_DockingEmptyBg] =
    ImVec4(0.20F, 0.20F, 0.20F, 1.00F);
  colors[ImGuiCol_PlotLines] =
    ImVec4(0.61F, 0.61F, 0.61F, 1.00F);
  colors[ImGuiCol_PlotLinesHovered] =
    ImVec4(0.14F, 0.87F, 0.05F, 1.00F);
  colors[ImGuiCol_PlotHistogram] =
    ImVec4(0.30F, 0.60F, 0.10F, 1.00F);
  colors[ImGuiCol_PlotHistogramHovered] =
    ImVec4(0.23F, 0.78F, 0.02F, 1.00F);
  colors[ImGuiCol_TableHeaderBg] =
    ImVec4(0.27F, 0.27F, 0.27F, 1.00F);
  colors[ImGuiCol_TableBorderStrong] =
    ImVec4(0.31F, 0.31F, 0.35F, 1.00F);
  colors[ImGuiCol_TableBorderLight] =
    ImVec4(0.23F, 0.23F, 0.25F, 1.00F);
  colors[ImGuiCol_TableRowBg] =
    ImVec4(0.00F, 0.00F, 0.00F, 0.00F);
  colors[ImGuiCol_TableRowBgAlt] =
    ImVec4(0.46F, 0.47F, 0.46F, 0.06F);
  colors[ImGuiCol_TextLink] =
    ImVec4(0.26F, 0.59F, 0.98F, 1.00F);
  colors[ImGuiCol_TextSelectedBg] =
    ImVec4(0.26F, 0.59F, 0.98F, 0.35F);
  colors[ImGuiCol_DragDropTarget] =
    ImVec4(1.00F, 1.00F, 0.00F, 0.90F);
  colors[ImGuiCol_NavCursor] =
    ImVec4(0.26F, 0.59F, 0.98F, 1.00F);
  colors[ImGuiCol_NavWindowingHighlight] =
    ImVec4(1.00F, 1.00F, 1.00F, 0.70F);
  colors[ImGuiCol_NavWindowingDimBg] =
    ImVec4(0.78F, 0.69F, 0.69F, 0.20F);
  colors[ImGuiCol_ModalWindowDimBg] =
    ImVec4(0.80F, 0.80F, 0.80F, 0.35F);
}