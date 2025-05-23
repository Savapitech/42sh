/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <unistd.h>

#include "key_handler.h"

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

bool handle_key_ctrl_f(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff)
{
    return handle_key_arrow_right(rh, ec, buff);
}

bool handle_key_ctrl_k(readline_helper_t *rh, exec_ctx_t *, buff_t *buff)
{
    if (!buff->sz && !rh->cursor)
        return false;
    buff->sz = rh->cursor;
    refresh_line(rh);
    return false;
}
