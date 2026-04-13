include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG "7b6aead9fb88b3623e3b3725ebb42670cbe4c579"
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_BRANCH "docking"
  GIT_TAG "62275e877a691bd2e51d1ca86bdfed49fa5cb297"
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui)
set(imgui_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/imgui-src")

add_library(imgui_glfw STATIC
  "${imgui_SOURCE_DIR}/imgui.cpp"
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
  "${imgui_SOURCE_DIR}/imgui_demo.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui_glfw
  PUBLIC
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
)

target_link_libraries(imgui_glfw PUBLIC glfw)