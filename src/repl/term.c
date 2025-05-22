/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#include <stdio.h>
#include <unistd.h>

#include "builtins_handler.h"
#include "u_str.h"
#include "vt100_esc_codes.h"

void init_shell_repl(exec_ctx_t *ec)
{
    struct termios new_settings = ec->saved_term_settings;

    ec->is_running = true;
    if (ec->isatty) {
        setvbuf(stdout, nullptr, _IONBF, 0);
        WRITE_CONST(STDOUT_FILENO, BLINKING_VERTICAL_CURSOR);
        new_settings.c_iflag &= ~IXON;
        new_settings.c_lflag &= ~(ECHO | ICANON | ISIG);
        new_settings.c_cc[VMIN] = 1;
        new_settings.c_cc[VTIME] = 0;
        tcsetattr(ec->read_fd, TCSANOW, &new_settings);
    }
}

void restore_term_flags(exec_ctx_t *exec_ctx)
{
    if (exec_ctx->isatty)
        tcsetattr(exec_ctx->read_fd, TCSANOW, &exec_ctx->saved_term_settings);
}
