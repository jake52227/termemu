#include <pty.h>
#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "pty.hpp"
#include "macros.hpp"
#include "window.hpp"

static void at_exit(int status, void *arg) {
    int *shell_proc = (int *) arg;
    kill(*shell_proc, SIGTERM);
}

// turn off echo for secondary pty 
static void disable_echo(int fd) {
    struct termios stermios;
    if (tcgetattr(fd, &stermios) < 0)
        errExit("tcgetattr error");

    stermios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    stermios.c_oflag &= ~(ONLCR);

    if (tcsetattr(fd, TCSANOW, &stermios) < 0)
        errExit("tcsetattr error");
}


void fork_pty_shell(int *pty_primary, int *shell_process_id) {
    const char *shell;
    char pty_secondary_name[1000];

    // Im not sure if this even does anything but i'll keep it here for now 
    unsigned short ws_row = getWindowWidth() / PIXEL_SIZE; // rows in characters
    unsigned short ws_col = getWindowHeight() / PIXEL_SIZE; // columns in characters
    struct winsize winp = (struct winsize) {
        ws_row,
            ws_col,
            0, // ws_xpixel is not used
            0  // ws_ypixel is not used either
    };

    int rc = forkpty(pty_primary, pty_secondary_name, NULL, &winp);
    if (rc < 0) {
        errExit("forkpty");
    } else if (rc == 0) { // child
        int fd = open(pty_secondary_name, O_RDWR);
        disable_echo(fd);
        dup2(STDIN_FILENO, fd);
        dup2(STDOUT_FILENO, fd);
        dup2(STDERR_FILENO, fd);
        
        // exec user's default shell and if that is not found, exec sh
        shell = getenv("SHELL");
        if (shell == NULL || *shell == '\0')
            shell = "/bin/sh";

        execlp(shell, shell, (char *) NULL);
        errExit("execlp");
    } else {
        *shell_process_id = rc;
        // register on_exit to kill shell process at exit
        if (on_exit(at_exit, (void *) shell_process_id) != 0) {
            kill(*shell_process_id, SIGTERM);
            errExit("on_exit");
        }   
    }
}
