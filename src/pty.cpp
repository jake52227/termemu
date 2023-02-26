#include <pty.h>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "pty.hpp"
#include "macros.hpp"

static void at_exit(int status, void *arg)
{
    int *shell_proc = (int *)arg;
    kill(*shell_proc, SIGTERM);
}

void fork_pty_shell(int *pty_primary, int *shell_process_id)
{
    const char *shell;
    char pty_secondary_name[1000];

    struct winsize winp {};

    int rc = forkpty(pty_primary, pty_secondary_name, NULL, &winp);
    if (rc < 0)
    {
        errExit("forkpty");
    }
    else if (rc == 0)
    { // child
        int fd = open(pty_secondary_name, O_RDWR);
        dup2(STDIN_FILENO, fd);
        dup2(STDOUT_FILENO, fd);
        dup2(STDERR_FILENO, fd);

        // exec user's default shell and if that is not found, exec sh
        shell = getenv("SHELL");
        if (shell == NULL || *shell == '\0')
            shell = "/bin/sh";

        execlp(shell, shell, (char *)NULL);
        errExit("execlp");
    }
    else
    {
        *shell_process_id = rc;
        // register on_exit to kill shell process at exit
        if (on_exit(at_exit, (void *)shell_process_id) != 0)
        {
            kill(*shell_process_id, SIGTERM);
            errExit("on_exit");
        }
    }
}
