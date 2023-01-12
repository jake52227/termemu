#include <cstdlib>
#include <iostream>
#include <pty.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <termios.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sys/wait.h>
#include "emulator.hpp"
#include "renderer.hpp"
#include "user_input.hpp"
#include "shell.hpp"
#include "macros.hpp"
#include "window.hpp"
#include "config.hpp"
#include "parser.hpp"

// TODO: this should probably be somewhere else
static UserInput inputBuffer;
static void char_callback(GLFWwindow *window, unsigned codepoint);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Emulator::Emulator(unsigned width, unsigned height, const char *title)
{
    make_window(width, height, title);
    glfwSetCharCallback(getWindow(), char_callback);
    glfwSetKeyCallback(getWindow(), key_callback);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        
        if (key == GLFW_KEY_ENTER)
        {
            inputBuffer.prepare_command();
        }
        else if (key == GLFW_KEY_BACKSPACE)
        {
            inputBuffer.erase_symbol();
        }
        else if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

static void char_callback(GLFWwindow *window, unsigned codepoint)
{
    inputBuffer.write_symbol((char)codepoint);
}

Shader setup_shader()
{
    Shader shader;
    shader.compile("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
    return shader;
}

void setup_projection(Shader &shader)
{
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(getWindowWidth()), 0.0f, static_cast<float>(getWindowHeight()));
    glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void pre_render()
{
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Emulator::start()
{
    Shader shader = setup_shader();
    Config cfg;
    Renderer renderer = Renderer(cfg);
    Shell shell;
    Parser parser;
    GLFWwindow *win = getWindow();

    float x;
    float y;

    while (!glfwWindowShouldClose(win))
    {
        // Wait for something to happen and don't render needlessly
        glfwWaitEvents();

        x = 10.0f;
        y = getWindowHeight() - PIXEL_SIZE;

        shader.use();
        setup_projection(shader);
        pre_render();

        if (inputBuffer.is_command_ready())
        {
            std::string command = inputBuffer.get_command();
            shell.write_to(command);
        }

        const std::string text = shell.read_from();

        if (!text.empty())
        {
            renderer.render(shader, parser, text, x, y);
        }
        else
        {
            const std::string buf(inputBuffer.get_buffer());
            renderer.render(shader, parser, buf, x, y);
        }

        glfwSwapBuffers(win);
    }
}

Emulator::~Emulator()
{
    glfwTerminate();
}
