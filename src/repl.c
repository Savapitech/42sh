/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <signal.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "common.h"
#include "repl.h"
#include "u_str.h"
#include "vt100_esc_codes.h"


void print_shell_prompt(exec_ctx_t *exec_ctx)
{
    env_t *env_ptr = exec_ctx->env;
    char const *ps1 = get_env_value(env_ptr, "PS1");
    char hostname[64];

    if (ps1 == nullptr) {
        if (gethostname(hostname, 64) < 0)
            return;
        printf(BLUE PROMPT_HEADER GREEN "%s" RESET "@" CYAN "%s" BLUE "] "
            RESET "-" BLUE " [" RESET "%s" BLUE
            "] " RESET "-" BLUE " [" YELLOW "%d" BLUE
            "]\n└─[" PURPLE "$" BLUE "] " RESET,
            get_env_value(env_ptr, "USER"),
            hostname,
            get_env_value(env_ptr, "PWD"),
            exec_ctx->history_command->sz);
    } else
        printf("%s", ps1);
}

void init_shell_repl(exec_ctx_t *exec_ctx)
{
    exec_ctx->is_running = true;
    if (isatty(exec_ctx->read_fd)) {
        setvbuf(stdout, nullptr, _IONBF, 0);
        signal(SIGINT, SIG_IGN);
        WRITE_CONST(STDOUT_FILENO, BLINKING_VERTICAL_CURSOR);
        exec_ctx->saved_term_settings.c_iflag = IXON;
        exec_ctx->saved_term_settings.c_lflag = ~(ECHO | ICANON);
        tcsetattr(exec_ctx->read_fd, TCSANOW, &exec_ctx->saved_term_settings);
    }
}

void restore_term_flags(exec_ctx_t *exec_ctx)
{
    if (!isatty(exec_ctx->read_fd))
        return;
    tcsetattr(exec_ctx->read_fd, TCSANOW, &exec_ctx->saved_term_settings);
}

static
void ignore_sigint(exec_ctx_t *exec_ctx)
{
    WRITE_CONST(exec_ctx->read_fd, "\n");
    print_shell_prompt(exec_ctx);
}

bool handle_keys(exec_ctx_t *exec_ctx, buff_t *buff, char const *read_buff)
{
    switch (*read_buff) {
        case CTRL('d'):
            buff->sz = 0;
            return true;
        case CTRL('c'):
            ignore_sigint(exec_ctx);
            return false;
        case CTRL('l'):
            WRITE_CONST(STDOUT_FILENO, ESC "[2J");
            WRITE_CONST(STDOUT_FILENO, ESC "[H");
            print_shell_prompt(exec_ctx);
            return false;
        default:
            return false;
    }
}
