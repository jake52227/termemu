#include <iostream>
#include <cstring>
#include "user_input.hpp"

UserInput::UserInput()
{
    bzero(this->buf, sizeof this->buf);
}

std::string UserInput::get_command()
{
    if (!this->command_ready)
        return "";

    this->command_ready = false;
    return this->command;
}

void UserInput::prepare_command()
{
    this->command = std::string(this->buf);
    bzero(this->buf, sizeof this->buf);
    this->buf_index = 0;
    this->command_ready = true;
}


bool UserInput::is_command_ready()
{
    return this->command_ready;
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
