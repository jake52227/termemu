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

void Emulator::write_to_shell_if_ready()
{
    if (UserInput::is_command_ready())
    {
        std::string command = UserInput::get_command();

        if (command.compare("clear") == 0)
            textContainer->clearStorage();
        
        textContainer->store(command);
        shell->write_to(command);
    }
}

std::unique_ptr<Emulator> Emulator::make(unsigned window_width, unsigned window_height, const char *window_title) {
    std::unique_ptr<Emulator> emulator = std::make_unique<Emulator>();
    Window::initialize(window_width, window_height, window_title);
    UserInput::initialize();
    Config config = Config();
    emulator->renderer = Renderer::create(config);
    emulator->shell = Shell::create(); 
    emulator->parser = Parser::create();
    emulator->textContainer = TextContainer::create();
    emulator->shader = Shader::create("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
    return emulator;
}

void Emulator::start()
{
    GLFWwindow *win = Window::getWindow();
    while (!glfwWindowShouldClose(win))
    {
        glfwWaitEvents();
        write_to_shell_if_ready(); 
        const std::string text = shell->read_from();
        if (!text.empty())
        {
            textContainer->store(text);
        }
        else
        {
            // TODO: create a parser to interpret the ansi codes and render the shell output
            // Test:
            this->renderer->clear_screen();
            const std::string buf(UserInput::get_buffer());
            const glm::vec3 color = glm::vec3(1.0f, 1.0, 1.0f);
            this->shader->set_text_color(color);
            renderer->render_text(buf, PIXEL_SIZE, PIXEL_SIZE, 1);
        }
        glfwSwapBuffers(win);
    }
}

Emulator::~Emulator()
{
    glfwTerminate();
}
