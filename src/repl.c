/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <signal.h>
#include <termios.h>
#include <unistd.h>

#include "exec.h"
#include "u_str.h"

void init_shell_repl(exec_ctx_t *exec_ctx)
{
    struct termios repl_settings;

    signal(SIGINT, SIG_IGN);
    exec_ctx->is_running = true;
    if (isatty(STDIN_FILENO)) {
        tcgetattr(STDIN_FILENO, &repl_settings);
        exec_ctx->saved_term_settings = repl_settings;
        repl_settings.c_iflag = IXON;
        repl_settings.c_lflag = ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &repl_settings);
    }
}

void restore_term_flags(exec_ctx_t *exec_ctx)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &exec_ctx->saved_term_settings);
}

static
void ignore_sigint(void)
{
    WRITE_CONST(STDIN_FILENO, "\n");
    WRITE_CONST(STDOUT_FILENO, SHELL_PROMPT);
}

bool handle_keys(buff_t *buff, char *read_buff)
{
    switch (*read_buff) {
        case CTRL('d'):
            buff->sz = 0;
            return true;
        case CTRL('c'):
            ignore_sigint();
            return false;
        default:
            return false;
    }
}
