#include <cstdlib>
#include <iostream>
#include "window.hpp"
#include "macros.hpp"

// TODO: perhaps not the best solution to have these here?
static GLFWwindow *WINDOW;
static unsigned WINDOW_HEIGHT;
static unsigned WINDOW_WIDTH;

unsigned getWindowHeight()
{
	return WINDOW_HEIGHT;
}
unsigned getWindowWidth()
{
	return WINDOW_HEIGHT;
}

GLFWwindow *getWindow()
{
	return WINDOW;
}

void make_window(unsigned width, unsigned height, const char *title)
{
	glfwInit();
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	WINDOW = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (WINDOW == nullptr)
	{
		glfwTerminate();
		errExit("window creation failed");
	}

	glfwMakeContextCurrent(WINDOW);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		errExit("failed to load function pointers with GLAD");

	glViewport(0, 0, width, height);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetFramebufferSizeCallback(WINDOW, framebuffer_size_callback);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);
}
