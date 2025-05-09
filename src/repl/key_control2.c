/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <string.h>
#include <unistd.h>

#include "key_handler.h"
#include "repl.h"
#include "vt100_esc_codes.h"

bool handle_key_ctrl_f(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff)
{
    return handle_key_arrow_right(rh, ec, buff);
}

bool handle_key_ctrl_l(readline_helper_t *, exec_ctx_t *ec, buff_t *)
{
    WRITE_CONST(STDOUT_FILENO, ESC "[2J");
    WRITE_CONST(STDOUT_FILENO, ESC "[H");
    print_shell_prompt(ec);
    return false;
}

bool handle_backspace(readline_helper_t *rh, exec_ctx_t *, buff_t *buff)
{
    if (rh->cursor == 0)
        return false;
    rh->cursor--;
    memmove(&buff->str[rh->cursor], &buff->str[rh->cursor + 1],
        buff->sz - rh->cursor);
    buff->sz--;
    refresh_line(rh);
    return false;
}

bool handle_delete(readline_helper_t *rh, exec_ctx_t *, buff_t *buff)
{
    if (rh->cursor >= buff->sz)
        return false;
    memmove(&buff->str[rh->cursor], &buff->str[rh->cursor + 1],
        buff->sz - rh->cursor);
    buff->sz--;
    refresh_line(rh);
    return false;
}
