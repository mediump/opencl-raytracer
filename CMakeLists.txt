cmake_minimum_required(VERSION 3.1)
project(OpenCLRaytracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCL REQUIRED)
add_compile_definitions(CL_TARGET_OPENCL_VERSION=300)

find_package(X11 REQUIRED)

add_executable(OpenCLRaytracer src/main.cpp
  src/opencl/cl_context.cpp
  src/opencl/cl_context.h
  src/utils/utils.cpp
  src/utils/utils.h
  src/os/window.cpp
  src/os/window.h
  src/app/state.h
  src/app/gui.cpp
  src/app/gui.h
  src/app/style.h
  src/app/utils/image.cpp
  src/app/utils/image.h
  src/app/renderer/renderer.cpp
  src/app/renderer/renderer.h
)

include(cmake/3rd.cmake)
add_subdirectory(3rd)
message("Linking libraries: ${FETCHCONTENT_LIBS} ${SOURCE_LIBS}")

target_link_libraries(OpenCLRaytracer
  PRIVATE
    OpenCL::OpenCL
    X11::X11
    glad
    glfw
    imgui_glfw
)

target_include_directories(OpenCLRaytracer
  PRIVATE
    src
)

# Copy kernel modules
file(GLOB_RECURSE CL_KERNELS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cl")
add_custom_command(
  TARGET OpenCLRaytracer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/kernels"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CL_KERNELS} "${CMAKE_BINARY_DIR}/kernels"
  COMMENT "Copying kernel files from '${CL_KERNELS}' -> '${CMAKE_BINARY_DIR}/kernels'"
  VERBATIM
)
