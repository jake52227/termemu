#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include "user_input.hpp"

UserInput::UserInput()
{
    bzero(this->buf, sizeof this->buf);
    this->pressed_key = -1;
}

// TODO: handle control keys and other missing keys
void UserInput::handle_regular_char_event(int key, int mods)
{
    if (key >= 65 && key <= 90)
    {
        if (mods == GLFW_MOD_SHIFT)
        {
            // uppercase
            this->set_pressed_key(key);
        }
        else
        {
            // lowercase
            this->set_pressed_key(key + 32);
        }
    }
    else
    {
        this->set_pressed_key(key);
    }
}

void UserInput::handle_key_event(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key != GLFW_KEY_BACKSPACE && key != GLFW_KEY_ENTER)
        {
            this->handle_regular_char_event(key, mods);
        }
        else
        {
            this->set_pressed_key(key);
        }
    }
}

void UserInput::set_pressed_key(int key)
{
    this->pressed_key = key;
}

std::string UserInput::get_command_if_ready()
{
    if (this->buf_index <= 1)
        return "";
    int latest = this->buf[this->buf_index - 1];
    // TODO: figure out why this is 1 instead of GLFW_ENTER
    if (latest == 1)
    {
        std::string cmd(this->buf);
        bzero(this->buf, sizeof this->buf);
        this->buf_index = 0;
        return cmd;
    }
    return "";
}

void UserInput::handle_key_event(int key)
{
    switch (key)
    {
    case GLFW_KEY_BACKSPACE:
        this->erase_symbol();
        break;

    default:
        this->write_symbol((char)key);
        break;
    }
    this->pressed_key = -1;
}

void UserInput::update()
{
    int key = this->pressed_key;
    if (key == -1)
        return;
    this->handle_key_event(key);
}

void UserInput::erase_symbol()
{
    if (this->buf_index == 0)
        return;
    this->buf_index--;
    this->buf[buf_index] = ' ';
    this->buf[buf_index + 1] = '\0';
}

void UserInput::write_symbol(char sym)
{
    if (buf_index >= 1022)
        return;
    this->buf[buf_index++] = sym;
    this->buf[buf_index] = '\0';
}

char *UserInput::get_buffer()
{
    char *buf_addr = this->buf;
    return buf_addr;
}
