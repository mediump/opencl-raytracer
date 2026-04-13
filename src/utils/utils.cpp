#include "utils.h"

#include <sstream>

std::string load_text_file(const char* filename)
{
  std::stringstream content;
  std::ifstream file(filename);

  if (!file.is_open()) {
    FATAL("Unable to open file: %s", filename);
    EXIT(1);
  }

  std::string line;
  while (std::getline(file, line)) {
    content << line << '\n';
  }

  file.close();

  return content.str();
}