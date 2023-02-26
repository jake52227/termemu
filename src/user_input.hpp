#ifndef INPUT_HPP
#define INPUT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <memory>
#include "renderer.hpp"

// heavy use of static methods due to callback functions 
class UserInput
{
public:
    static void initialize();
    static char *get_buffer();
    static void erase_symbol();
    static void write_symbol(char sym);
    static void prepare_command();
    static bool is_command_ready();
    static std::string get_command();
private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void char_callback(GLFWwindow *window, unsigned codepoint);
    static char buf[1024];
    static bool command_ready;
    static std::string command;
    static unsigned int buf_index;
};

#endif
