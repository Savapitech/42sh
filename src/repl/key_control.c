/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <unistd.h>

#include "key_handler.h"
#include "repl.h"
#include "vt100_esc_codes.h"

bool handle_key_ctrl_c(exec_ctx_t *ec, buff_t *)
{
    WRITE_CONST(ec->read_fd, "\n");
    print_shell_prompt(ec);
    return false;
}

bool handle_key_ctrl_d(exec_ctx_t *, buff_t *buff)
{
    buff->sz = 0;
    return true;
}

bool handle_key_ctrl_l(exec_ctx_t *ec, buff_t *)
{
    WRITE_CONST(STDOUT_FILENO, ESC "[2J");
    WRITE_CONST(STDOUT_FILENO, ESC "[H");
    print_shell_prompt(ec);
    return false;
}

bool handle_delete(exec_ctx_t *, buff_t *buff)
{
    if (buff->sz > 0)
        buff->sz--;
    return false;
}
