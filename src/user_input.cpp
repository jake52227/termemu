#include <iostream>
#include <cstring>
#include <stdexcept>        
#include "user_input.hpp"

char UserInput::buf[1024];
bool UserInput::command_ready;
std::string UserInput::command;
unsigned int UserInput::buf_index;

void UserInput::initialize()
{
    if (Window::getWindow() == nullptr) {
        throw std::invalid_argument("Window needs to be initialized first");
    }
    UserInput::command_ready = false;
    UserInput::buf_index = 0;
    bzero(UserInput::buf, 1024*sizeof(char));
    glfwSetCharCallback(Window::getWindow(), UserInput::char_callback);
    glfwSetKeyCallback(Window::getWindow(), UserInput::key_callback);
}

void UserInput::char_callback(GLFWwindow *window, unsigned codepoint)
{
    UserInput::write_symbol((char)codepoint);
}

void UserInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ENTER)
        {
            UserInput::prepare_command();
        }
        else if (key == GLFW_KEY_BACKSPACE)
        {
            UserInput::erase_symbol();
        }
        else if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

std::string UserInput::get_command()
{
    if (!UserInput::command_ready)
        return "";

    UserInput::command_ready = false;
    return UserInput::command;
}

void UserInput::prepare_command()
{
    UserInput::command = std::string(UserInput::buf);
    bzero(UserInput::buf, sizeof UserInput::buf);
    UserInput::buf_index = 0;
    UserInput::command_ready = true;
}

bool UserInput::is_command_ready()
{
    return UserInput::command_ready;
}

void UserInput::erase_symbol()
{
    if (UserInput::buf_index == 0)
        return;
    UserInput::buf_index--;
    UserInput::buf[buf_index] = ' ';
    UserInput::buf[buf_index + 1] = '\0';
}

void UserInput::write_symbol(char sym)
{
    if (buf_index >= 1022)
        return;
    UserInput::buf[buf_index++] = sym;
    UserInput::buf[buf_index] = '\0';
}

char *UserInput::get_buffer()
{
    char *buf_addr = UserInput::buf;
    return buf_addr;
}
