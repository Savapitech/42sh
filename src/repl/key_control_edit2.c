/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "key_handler.h"
#include "u_str.h"

bool handle_key_ctrl_w(readline_helper_t *rh, exec_ctx_t *, buff_t *buff)
{
    if (!rh->cursor || !buff->sz)
        return false;
    rh->ec->paste_buff = u_strndup(buff->str, rh->cursor);
    if (rh->ec->paste_buff == nullptr)
        return true;
    memmove(buff->str, buff->str + rh->cursor, rh->cursor);
    buff->sz -= rh->cursor;
    rh->cursor = 0;
    refresh_line(rh);
    return false;
}

bool handle_key_ctrl_y(readline_helper_t *rh, exec_ctx_t *, buff_t *buff)
{
    size_t paste_len;

    if (rh->ec->paste_buff == nullptr)
        return false;
    paste_len = strlen(rh->ec->paste_buff);
    if (!ensure_buff_av_capacity(buff, paste_len))
        return true;
    strcpy(buff->str + paste_len, rh->ec->paste_buff);
    buff->sz += paste_len - 2;
    rh->cursor += paste_len;
    return false;
}
