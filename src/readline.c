/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "readline.h"
#include "debug.h"
#include "repl.h"
#include "u_str.h"
#include "vt100_esc_codes.h"

bool ensure_buff_av_capacity(buff_t *buff, size_t requested)
{
    char *new_str;
    size_t endsize = BUFF_INIT_SZ;

    if (buff->str == NULL) {
        buff->str = malloc((sizeof *buff->str) * requested);
        if (buff->str == NULL)
            return false;
        buff->cap = requested;
    }
    if ((buff->sz + requested) < buff->cap)
        return true;
    for (; endsize < buff->sz + requested; endsize <<= 1);
    if (endsize > buff->cap) {
        new_str = realloc(buff->str, (sizeof *buff->str) * endsize);
        if (new_str == NULL)
            return false;
        buff->str = new_str;
        buff->cap = endsize;
    }
    return true;
}

static
bool append_null_terminator(buff_t *out)
{
    if (out->sz == 0)
        return true;
    if (!ensure_buff_av_capacity(out, 1))
        return false;
    if (out->str[out->sz - 1] == '\n')
        out->sz--;
    out->str[out->sz] = '\0';
    return true;
}

static
bool copy_single_char(
    readline_helper_t *rh,
    buff_t *out,
    char *cpy,
    char in)
{
    if (in == '\r')
        in = '\n';
    if (in == '\0' || in == '\f')
        return false;
    if (isspace(in) || isprint(in) || !rh->ec->isatty) {
        *cpy = in;
        if (rh->cursor != rh->out->sz && in == '\n')
            return true;
        memmove(&out->str[rh->cursor + 1], &out->str[rh->cursor],
            out->sz - rh->cursor);
        out->str[rh->cursor] = in;
        out->sz++;
        rh->cursor++;
        refresh_line(rh);
        return true;
    }
    return false;
}

void write_buff(readline_helper_t *rh)
{
    char move_back[32] = {0};

    if (!rh->ec->isatty)
        return;
    WRITE_CONST(STDOUT_FILENO, ERASE_TO_END_LINE);
    write(STDOUT_FILENO, &rh->out->str[rh->cursor], rh->out->sz - rh->cursor);
    snprintf(move_back, sizeof(move_back), "\033[%zuD",
        rh->out->sz - rh->cursor);
    write(STDOUT_FILENO, move_back, strlen(move_back));
}

static
void print_buff(readline_helper_t *rh)
{
    size_t target = rh->cursor + rh->ec->prompt_len;
    char move_cursor[32];

    WRITE_CONST(STDOUT_FILENO, "\r");
    if (rh->ec->prompt_len > 0)
        print_second_shell_prompt(rh->ec);
    write(STDOUT_FILENO, rh->out->str, rh->out->sz);
    WRITE_CONST(STDOUT_FILENO, ERASE_TO_END_LINE);
    snprintf(move_cursor, sizeof move_cursor, "\r\033[%zuC", target);
    write(STDOUT_FILENO, move_cursor, strlen(move_cursor));
}

void refresh_line(readline_helper_t *rh)
{
    if (!rh->ec->isatty)
        return;
    append_null_terminator(rh->out);
    if (rh->cursor + rh->ec->prompt_len > rh->winsz.ws_col) {
        write(STDOUT_FILENO, rh->out->str + rh->cursor - 1, 1);
        return;
    }
    if (rh->out->sz > 1 && *rh->cpy == '\n') {
        WRITE_CONST(STDOUT_FILENO, "\n");
        return;
    }
    print_buff(rh);
}

static
bool populate_copy_buff(
    readline_helper_t *rh, ssize_t rd,
    text_parse_info_t *tpi)
{
    ssize_t i = 0;
    ssize_t skip;

    for (bool done = false; !done && i < rd; i++) {
        skip = handle_keys(rh, rh->out, &rh->in[i], BULK_READ_BUFF_SZ - i);
        if (skip < 0) {
            tpi->used = i;
            return false;
        }
        if (skip) {
            i += skip - 1;
            continue;
        }
        if (copy_single_char(rh, rh->out, &rh->cpy[tpi->written], rh->in[i])) {
            done = rh->cpy[tpi->written] == '\n';
            tpi->written++;
            continue;
        }
    }
    refresh_line(rh);
    tpi->used = i;
    return true;
}

static
bool read_until_line_ending(
    exec_ctx_t *ec, readline_helper_t *rh, ssize_t rd)
{
    text_parse_info_t tpi;

    for (;;) {
        if (ec->isatty)
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &rh->winsz);
        memset(&tpi, '\0', sizeof tpi);
        if (!ensure_buff_av_capacity(rh->out, BULK_READ_BUFF_SZ))
            return false;
        if (!populate_copy_buff(rh, rd, &tpi))
            return true;
        memmove(rh->in, &rh->in[tpi.used], BULK_READ_BUFF_SZ - tpi.used);
        memset(&rh->in[BULK_READ_BUFF_SZ - tpi.used], '\0', tpi.used);
        if (rh->cpy[tpi.written - 1] == '\n')
            break;
        rd = read(ec->read_fd, rh->in, BULK_READ_BUFF_SZ);
        if (rd <= 0)
            return (rd == 0);
    }
    return true;
}

bool readline(exec_ctx_t *ec, buff_t *out)
{
    static char read_buff[BULK_READ_BUFF_SZ] = {0};
    readline_helper_t rh = { ec, out, read_buff, { 0 }, 0,
        .history_idx = ec->history_command->sz, .glob = {0} };
    bool is_empty = true;
    ssize_t rd = 0;

    if (ec->isatty)
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &rh.winsz);
    for (size_t i = 0; i < sizeof read_buff; i++)
        is_empty &= read_buff[i] == '\0';
    if (is_empty) {
        rd = read(ec->read_fd, read_buff, sizeof read_buff);
        if (rd < 0)
            return (rd == 0);
    } else
        rd = BULK_READ_BUFF_SZ;
    if (!read_until_line_ending(ec, &rh, rd))
        return false;
    return append_null_terminator(out);
}
