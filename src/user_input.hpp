#ifndef INPUT_HPP
#define INPUT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include "renderer.hpp"

class UserInput
{
public:
    UserInput();
    char *get_buffer();
    void erase_symbol();
    void write_symbol(char sym);
    void prepare_command();
    bool is_command_ready();
    std::string get_command();
    // TODO: add text cursor movement
private:
    char buf[1024];
    bool command_ready {};
    std::string command {};
    unsigned int buf_index{0};
};

#endif
