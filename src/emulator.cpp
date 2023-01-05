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
#include "text_buffer.hpp"
#include "shell.hpp"
#include "macros.hpp"
#include "window.hpp"
#include "config.hpp"

// TODO: this should probably be somewhere else
static TextBuffer textBuffer;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Emulator::Emulator(unsigned width, unsigned height, const char *title) {
    make_window(width, height, title);
    glfwSetKeyCallback(getWindow(), key_callback);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // temporary way to exit more easily without sigkill:
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }
    textBuffer.handle_key_event(key, scancode, action, mods);
}

Shader setup_shader() {
    Shader shader;
    shader.compile("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
    return shader;
}

void setup_projection(Shader &shader) {
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(getWindowWidth()), 0.0f, static_cast<float>(getWindowHeight()));
    glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void pre_render() {
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Emulator::start() {
    Shader shader = setup_shader();
    Config cfg;
    Renderer renderer = Renderer(cfg);
    Shell shell;
    GLFWwindow *win = getWindow();

    float x;
    float y;

    while(!glfwWindowShouldClose(win)) {
        // Wait for something to happen and don't render needlessly
        glfwWaitEvents();

        x = 10.0f;
        y = getWindowHeight() - 16;

        shader.use();
        setup_projection(shader);
        pre_render();

        textBuffer.update_state(renderer);
        std::string command = textBuffer.get_command_if_ready();

        if (!command.empty())
            shell.write_to(command);

        const std::vector<struct ParsedText> words = shell.read_from();

        if (words.size() > 0) {
            renderer.render_words(shader, words, x, y);
        } else {
            const char *buf = textBuffer.get_buffer();
            renderer.render_user_text(shader, buf, x, y);
        }

        glfwSwapBuffers(win);
    }
}

Emulator::~Emulator() {
    glfwTerminate();
}
