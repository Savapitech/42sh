/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "common.h"
#include "debug.h"
#include "repl.h"
#include "repl/key_handler.h"
#include "u_str.h"
#include "vt100_esc_codes.h"

const key_handler_t KEY_HANDLERS[] = {
    {"\03", handle_key_ctrl_c},  // ^C
    {"\04", handle_key_ctrl_d},  // ^D
    {"\014", handle_key_ctrl_l}, // ^L
    {ESC "[A", handle_key_arrow_up},
    {ESC "[B", handle_key_arrow_down},
    {ESC "[C", handle_key_arrow_right},
    {ESC "[D", handle_key_arrow_left},
    {"\x7f", handle_delete},
};

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

ssize_t handle_keys(
    exec_ctx_t *ec,
    buff_t *buff,
    char const *read_buff,
    size_t len)
{
    U_DEBUG("Found special char, [%hhx]\n", *read_buff);
    for (size_t i = 0; i < sizeof KEY_HANDLERS / sizeof *KEY_HANDLERS; i++) {
        if (strncmp(read_buff, KEY_HANDLERS[i].name,
                strlen(KEY_HANDLERS[i].name)) != 0)
            continue;
        if (!KEY_HANDLERS[i].exec(ec, buff))
            return strlen(KEY_HANDLERS[i].name);
        return -1;
    }
    for (size_t i = 0; i < len; i++)
        U_DEBUG("<- [%d]\n", read_buff[i]);
    return 1;
}
