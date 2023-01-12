#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>
#include "renderer.hpp"

class UserInput
{
public:
    UserInput();
    char *get_buffer();
    void update();
    void handle_key_event(int key, int scancode, int action, int mods);
    std::string get_command_if_ready();
    // TODO: add text cursor movement
private:
    int pressed_key;
    char buf[1024];
    unsigned int buf_index{0};
    void handle_key_event(int key);
    void set_pressed_key(int key);
    void handle_regular_char_event(int key, int mods);
    void erase_symbol();
    void write_symbol(char sym);
};

#endif
