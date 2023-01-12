#include <unistd.h>
#include <cstring>
#include <iostream>
#include "shell.hpp"
#include "pty.hpp"

Shell::Shell()
{
    fork_pty_shell(&(this->pty_primary), &(this->shell_proc));
}

Shell::~Shell()
{
    close(this->pty_primary);
}

void Shell::write_to(std::string cmd)
{
    const char *s = cmd.c_str();
    const char *ptr = s;
    unsigned length = 0;
    while (*ptr != '\0')
    {
        ++ptr;
        ++length;
    }
    ++length;
    char *buf = new char[length];
    memcpy(buf, s, sizeof(char) * (length - 1));
    // add newline (line buffering)
    buf[length - 1] = '\n';
    write(this->pty_primary, buf, sizeof(char) * length);
    delete[] buf;
}

std::string Shell::read_from()
{
    struct timeval tv
    {
        0, 0
    };
    // TODO: not safe to assume size of buffer -> change this to dynamic allocation
    char buf[4096];
    bzero(buf, sizeof buf);
    ssize_t size;
    size_t count = 0;

    FD_ZERO(&(this->rfds));
    FD_SET(this->pty_primary, &(this->rfds));
    if (select(this->pty_primary + 1, &(this->rfds), NULL, NULL, &tv))
    {
        size = read(this->pty_primary, buf, 4096);
        buf[size] = '\0';
        count += size;
    }
    else
    {
        buf[0] = '\0';
    }

    Parser psr;
    std::string out = std::string(buf);

    return out;
}
