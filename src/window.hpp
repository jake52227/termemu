#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define PIXEL_SIZE 16 // used by freetype

unsigned getWindowHeight();
unsigned getWindowWidth();
GLFWwindow *getWindow();
void make_window(unsigned width, unsigned height, const char *title);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif
