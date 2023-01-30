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
#include "text_container.hpp"
#include "draw_position.hpp"

// TODO: this should probably be somewhere else
static UserInput userInputBuffer;
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
            userInputBuffer.prepare_command();
        }
        else if (key == GLFW_KEY_BACKSPACE)
        {
            userInputBuffer.erase_symbol();
        }
        else if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

static void char_callback(GLFWwindow *window, unsigned codepoint)
{
    userInputBuffer.write_symbol((char)codepoint);
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

void write_to_shell_if_ready(TextContainer &textContainer, Shell &shell)
{
    if (userInputBuffer.is_command_ready())
    {
        std::string command = userInputBuffer.get_command();
        textContainer.store(command);
        shell.write_to(command);
    }
}

// draw to from starting position upward until the top of the screen is reached
void draw_up_to_ceiling(TextContainer &tc, DrawPos &dp, Renderer &ren, Shader &sh, Parser &p)
{
    dp.changeRow(true);
    auto latest_line = tc.getIteratorToLatest();
    auto oldest_line = tc.getIteratorToOldest();

    while (latest_line != oldest_line && !dp.atMaxY())
    {
        ren.render_line(sh, p, *latest_line, dp);
        dp.changeRow(true);
        ++latest_line;
    }
}

void Emulator::start()
{
    Shader shader = setup_shader();
    Config cfg;
    Renderer renderer = Renderer(cfg);
    Shell shell;
    Parser parser;
    TextContainer textContainer;
    GLFWwindow *win = getWindow();
    DrawPos drawingPos(PIXEL_SIZE, PIXEL_SIZE, getWindowWidth(), getWindowHeight(), PIXEL_SIZE, PIXEL_SIZE);

    while (!glfwWindowShouldClose(win))
    {
        // Wait for something to happen and don't render needlessly
        glfwWaitEvents();

        shader.use();
        setup_projection(shader);
        pre_render();

        write_to_shell_if_ready(textContainer, shell); 

        const std::string text = shell.read_from();

        if (!text.empty())
            textContainer.store(text);
        
        const std::string buf(userInputBuffer.get_buffer());

        // draw from bottom up. First the user input and on top of that the saved shell output from latest to oldest
        renderer.render_line(shader, parser, buf, drawingPos);
        draw_up_to_ceiling(textContainer, drawingPos, renderer, shader, parser);   

        drawingPos.resetX();
        drawingPos.resetY();
        
        glfwSwapBuffers(win);
    }
}

Emulator::~Emulator()
{
    glfwTerminate();
}
