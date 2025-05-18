/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "key_handler.h"
#include "readline.h"
#include "u_str.h"

static
bool cat_history_entry(buff_t *buff, size_t i, his_command_t *cmd_history)
{
    if (!ensure_buff_av_capacity(buff,
            strlen(cmd_history[i].command)))
        return false;
    strcpy(buff->str, cmd_history[i].command);
    if (buff->str == nullptr)
        return true;
    buff->sz = strlen(buff->str);
    if (cmd_history[i].arg != nullptr) {
        if (!ensure_buff_av_capacity(buff,
                strlen(cmd_history[i].arg) + 1))
            return false;
        buff->str[buff->sz] = ' ';
        strcat(buff->str, cmd_history[i].arg);
        buff->sz += strlen(cmd_history[i].arg) + 1;
    }
    return true;
}

bool handle_key_arrow_up(
    readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff)
{
    his_command_t *cmd_history = ec->history_command;

    if (cmd_history->sz && rh->history_idx) {
        rh->history_idx--;
        if (!cat_history_entry(buff, rh->history_idx, cmd_history))
            return false;
        rh->cursor = buff->sz;
        refresh_line(rh);
    }
    return false;
}

bool handle_key_arrow_down(
    readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff)
{
    his_command_t *cmd_history = ec->history_command;

    if (cmd_history->sz && rh->history_idx + 1 < cmd_history->sz) {
        rh->history_idx++;
        if (!cat_history_entry(buff, rh->history_idx, cmd_history))
            return false;
        rh->cursor = buff->sz;
        refresh_line(rh);
    }
    return false;
}

bool handle_key_arrow_left(
    readline_helper_t *rh, exec_ctx_t *, buff_t *)
{
    if (rh->cursor > 0) {
        rh->cursor--;
        refresh_line(rh);
    }
    U_DEBUG("Key [LEFT] cursor [%zd]\n", rh->cursor);
    return false;
}

bool handle_key_arrow_right(
    readline_helper_t *rh, exec_ctx_t *, buff_t *buff)
{
    if (rh->cursor < buff->sz) {
        rh->cursor++;
        refresh_line(rh);
    }
    U_DEBUG("Key [RIGHT] cursor [%zd]\n", rh->cursor);
    return false;
}
