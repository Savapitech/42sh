/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <unistd.h>

#include "key_handler.h"
#include "repl.h"

bool handle_key_ctrl_c(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff)
{
    WRITE_CONST(ec->read_fd, "\n");
    print_shell_prompt(ec);
    rh->cursor = 0;
    buff->sz = 0;
    return false;
}

bool handle_key_ctrl_d(readline_helper_t *, exec_ctx_t *ec, buff_t *buff)
{
    if (ec->ignoreof)
        return false;
    buff->sz = 0;
    return true;
}

bool handle_key_ctrl_e(readline_helper_t *rh, exec_ctx_t *, buff_t *buff)
{
    rh->cursor = buff->sz;
    refresh_line(rh);
    return false;
}

bool handle_key_ctrl_a(readline_helper_t *rh, exec_ctx_t *, buff_t *)
{
    rh->cursor = 0;
    refresh_line(rh);
    return false;
}

bool handle_key_ctrl_b(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff)
{
    return handle_key_arrow_left(rh, ec, buff);
}
