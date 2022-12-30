#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Emulator {
public:
  Emulator(unsigned window_width, unsigned window_height, const char *window_title);
  ~Emulator();
  void start();
};


#endif
