/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "common.h"
#include "debug.h"
#include "repl.h"
#include "repl/key_handler.h"
#include "u_str.h"
#include "visitor.h"
#include "vt100_esc_codes.h"

const key_handler_t KEY_HANDLERS[] = {
    {"\03", handle_key_ctrl_c},  // ^C
    {"\04", handle_key_ctrl_d},  // ^D
    {"\014", handle_key_ctrl_l}, // ^L
    {"\05", handle_key_ctrl_e}, // ^E
    {"\01", handle_key_ctrl_a}, // ^A
    {"\02", handle_key_ctrl_b}, // ^B
    {"\06", handle_key_ctrl_f}, // ^F
    {ESC "[A", handle_key_arrow_up},
    {ESC "[B", handle_key_arrow_down},
    {ESC "[C", handle_key_arrow_right},
    {ESC "[D", handle_key_arrow_left},
    {"\x7f", handle_backspace},
    {"\x1b[3~", handle_delete},
};

void print_second_shell_prompt(exec_ctx_t *ec)
{
    env_t *env_ptr = ec->env;
    char const *ps1 = get_env_value(env_ptr, "PS1");
    char const *username = get_env_value(env_ptr, "USER");

    if (username == nullptr)
        username = "?";
    if (ps1 == nullptr) {
        printf(BLUE "└─[" PURPLE "%s%s" BLUE "] " RESET,
            ec->history->last_exit_code == 0 ? "" : RED,
            strcmp(username, "root") == 0 ? "#" : "$");
        ec->prompt_len = 6;
    } else {
        printf("%s", ps1);
        ec->prompt_len = strlen(ps1);
    }
}

static
void print_prompt(env_t *env_ptr, char *hostname, exec_ctx_t *ec)
{
    char const *username = get_env_value(env_ptr, "USER");

    if (username == nullptr)
        username = "?";
    printf(BLUE PROMPT_HEADER GREEN "%s" RESET "@" CYAN "%s" BLUE "] "
        RESET "-" BLUE " [" RESET "%s" BLUE
        "] " RESET "-" BLUE " [" YELLOW "%d" BLUE
        "]\n└─[" PURPLE "%s%s" BLUE "] " RESET,
        username,
        hostname,
        get_env_value(env_ptr, "PWD"),
        ec->history_command->sz + 1,
        ec->history->last_exit_code == 0 ? "" : RED,
        strcmp(username, "root") == 0 ? "#" : "$");
}

void print_shell_prompt(exec_ctx_t *ec)
{
    env_t *env_ptr = ec->env;
    char const *ps1 = get_env_value(env_ptr, "PS1");
    char hostname[64];

    if (ec->precmd != nullptr)
        visitor(ec->precmd, ec);
    if (ps1 == nullptr) {
        if (gethostname(hostname, 64) < 0)
            return;
        print_prompt(env_ptr, hostname, ec);
        ec->prompt_len = 6;
    } else {
        printf("%s", ps1);
        ec->prompt_len = strlen(ps1);
    }
}

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

ssize_t handle_keys(
    readline_helper_t *rh,
    buff_t *buff,
    char const *read_buff,
    size_t len)
{
    for (size_t i = 0; i < sizeof KEY_HANDLERS / sizeof *KEY_HANDLERS; i++) {
        if (strncmp(read_buff, KEY_HANDLERS[i].name,
                strlen(KEY_HANDLERS[i].name)) != 0)
            continue;
        if (!KEY_HANDLERS[i].exec(rh, rh->ec, buff))
            return strlen(KEY_HANDLERS[i].name);
        return -1;
    }
    for (size_t i = 0; i < len; i++)
        U_DEBUG("<- [%d]\n", read_buff[i]);
    return 1;
}
