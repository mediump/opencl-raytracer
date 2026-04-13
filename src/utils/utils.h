#pragma once

#include <cstdio>
#include <fstream>
#include <string>

#include <GLFW/glfw3.h>

// logging

#define LOG_MSG(fmt, ...)                                  \
  printf(fmt, ##__VA_ARGS__);                              \
  printf("\n")

#define FATAL(fmt, ...)                                    \
  fprintf(stderr, "[FATAL_ERROR]: ");                      \
  fprintf(stderr, fmt, ##__VA_ARGS__)

// exiting

#define EXIT(n) exit(n)

// graphics

#define GLFW_VERSION(v_maj, v_min)                         \
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, v_maj);       \
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, v_min);

// file io

std::string load_text_file(const char* filename);