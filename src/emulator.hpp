#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.hpp"
#include "user_input.hpp"
#include "shell.hpp"
#include "macros.hpp"
#include "window.hpp"
#include "config.hpp"
#include "parser.hpp"
#include "text_container.hpp"
#include "draw_position.hpp"

class Emulator
{
public:
  Emulator(unsigned window_width, unsigned window_height, const char *window_title);
  ~Emulator();
  void start();
private:

};

#endif
