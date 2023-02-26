#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "renderer.hpp"
#include "user_input.hpp"
#include "shell.hpp"
#include "macros.hpp"
#include "window.hpp"
#include "config.hpp"
#include "parser.hpp"
#include "text_container.hpp"

class Emulator
{
public:
  static std::unique_ptr<Emulator> make(unsigned window_width, unsigned window_height, const char *window_title);
  ~Emulator();
  void start();
private:
  void write_to_shell_if_ready();
  void pre_draw();
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<Shader> shader;
  std::unique_ptr<Shell> shell;
  std::unique_ptr<Parser> parser;
  std::unique_ptr<TextContainer> textContainer;
};

#endif
