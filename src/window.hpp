#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define PIXEL_SIZE 16 // used by freetype

class Window
{
public:
    static void initialize(unsigned width, unsigned height, const char *title);
    static unsigned getWindowHeight();
    static GLFWwindow *getWindow();
    static unsigned getWindowWidth();
private:
    static unsigned windowHeight;
    static unsigned windowWidth;
    static GLFWwindow *win;
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};

#endif
