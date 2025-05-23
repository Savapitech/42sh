/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <stdlib.h>
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
    strcpy(buff->str + rh->cursor, rh->ec->paste_buff);
    buff->sz += paste_len;
    rh->cursor += paste_len;
    return false;
}

static
void display_reverse_search_prompt(
    const char *search, his_command_t *hist, ssize_t found)
{
    WRITE_CONST(STDOUT_FILENO, "\r\x1b[K");
    dprintf(STDOUT_FILENO, "(reverse-i-search)`%s': ", search);
    if (found != -1) {
        dprintf(STDOUT_FILENO, "%s", hist[found].command);
        if (hist[found].arg != NULL)
            dprintf(STDOUT_FILENO, " %s", hist[found].arg);
    }
}

static
ssize_t find_match_in_history(his_command_t *hist, const char *search)
{
    ssize_t i = hist->sz - 1;

    for (; i >= 0; --i)
        if (strstr(hist[i].command, search))
            return i;
    return -1;
}

static
void update_search_buffer(char *search, size_t *len, char c)
{
    if (c == 127 || c == '\b') {
        if (*len > 0) {
            (*len)--;
            search[*len] = '\0';
        }
    } else {
        search[*len] = c;
        (*len)++;
        search[*len] = '\0';
    }
}

static
bool set_found(
    readline_helper_t *rh,
    ssize_t found,
    buff_t *buff,
    his_command_t *hist)
{
    if (found != -1) {
        if (!cat_history_entry(buff, found, hist))
            return false;
        rh->cursor = buff->sz;
    }
    return true;
}

static
bool reverse_search_loop(
    readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff, char *search)
{
    size_t len = 0;
    char c;
    ssize_t found = -1;
    his_command_t *hist = ec->history_command;

    while (read(ec->read_fd, &c, 1) == 1) {
        if (c == '\n' || c == '\r')
            break;
        update_search_buffer(search, &len, c);
        found = find_match_in_history(hist, search);
        display_reverse_search_prompt(search, hist, found);
        if (!set_found(rh, found, buff, hist))
            return false;
    }
    return true;
}

bool handle_key_ctrl_r(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff)
{
    char search[1024] = {0};

    WRITE_CONST(STDOUT_FILENO, "\r(reverse-i-search): ");
    if (!reverse_search_loop(rh, ec, buff, search))
        return true;
    refresh_line(rh);
    return false;
}
