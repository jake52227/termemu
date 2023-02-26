#include <cstdlib>
#include <iostream>
#include "window.hpp"
#include "macros.hpp"

unsigned Window::windowHeight;
unsigned Window::windowWidth;
GLFWwindow *Window::win;

unsigned Window::getWindowHeight()
{
	return Window::windowHeight;
}
unsigned Window::getWindowWidth()
{
	return Window::windowWidth;
}

GLFWwindow *Window::getWindow()
{
	return Window::win;
}

void Window::initialize(unsigned width, unsigned height, const char *title)
{
	Window::win = nullptr;
	glfwInit();
	Window::windowWidth = width;
	Window::windowHeight = height;
	Window::win = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (Window::win  == nullptr)
	{
		glfwTerminate();
		errExit("window creation failed");
	}

	glfwMakeContextCurrent(Window::win);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		errExit("failed to load function pointers with GLAD");

	glViewport(0, 0, width, height);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetFramebufferSizeCallback(Window::win, Window::framebuffer_size_callback);
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	Window::windowWidth = width;
	Window::windowHeight = height;
	glViewport(0, 0, width, height);
}
