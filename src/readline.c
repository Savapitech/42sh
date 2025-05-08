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

#include "debug.h"
#include "readline.h"
#include "repl.h"
#include "u_str.h"

static
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
    U_DEBUG("buff sz: %zu\n", out->sz);
    if (out->str[out->sz - 1] == '\n')
        out->sz--;
    U_DEBUG("readline [%.*s]\n", (int)out->sz, out->str);
    out->str[out->sz] = '\0';
    out->sz++;
    return true;
}

static
bool populate_single_char(
    exec_ctx_t *exec_ctx,
    buff_t *out,
    char *cpy,
    char in)
{
    if (in == '\r')
        in = '\n';
    if (in == '\0')
        return false;
    if (isspace(in) || isprint(in) || !exec_ctx->isatty) {
        *cpy = in;
        out->str[out->sz] = in;
        out->sz++;
        return true;
    }
    return false;
}

static
bool populate_copy_buff(
    exec_ctx_t *exec_ctx, readline_helper_t *rh, ssize_t rd,
    text_parse_info_t *tpi)
{
    size_t w = 0;
    ssize_t i = 0;

    for (bool done = false; !done && i < rd; i++) {
        if (populate_single_char(exec_ctx, rh->out, &rh->cpy[w], rh->in[i])) {
            done = rh->cpy[w] == '\n';
            w++;
            continue;
        }
        if (handle_keys(exec_ctx, rh->out, &rh->in[i])) {
            tpi->used = i;
            tpi->written = w;
            return false;
        }
    }
    if (exec_ctx->isatty)
        write(STDOUT_FILENO, rh->cpy, w);
    tpi->used = i;
    tpi->written = w;
    return true;
}

static
bool read_until_line_ending(
    exec_ctx_t *exec_ctx, readline_helper_t *rh, ssize_t rd)
{
    text_parse_info_t tpi;

    for (;;) {
        memset(&tpi, '\0', sizeof tpi);
        if (!ensure_buff_av_capacity(rh->out, BULK_READ_BUFF_SZ))
            return false;
        populate_copy_buff(exec_ctx, rh, rd, &tpi);
        U_DEBUG("copied %zu chars to cpy (%zu used)\n", tpi.written, tpi.used);
        memmove(rh->in, &rh->in[tpi.used], BULK_READ_BUFF_SZ - tpi.used);
        memset(&rh->in[BULK_READ_BUFF_SZ - tpi.used], '\0', tpi.used);
        if (rh->cpy[tpi.written - 1] == '\n')
            break;
        rd = read(exec_ctx->read_fd, rh->in, BULK_READ_BUFF_SZ);
        if (rd <= 0)
            return (rd == 0);
        U_DEBUG("read %zu characters\n", rd);
    }
    return true;
}

bool readline(exec_ctx_t *exec_ctx, buff_t *out)
{
    static char read_buff[BULK_READ_BUFF_SZ] = {0};
    readline_helper_t rh = { out, read_buff, { 0 } };
    bool is_empty = true;
    ssize_t rd = 0;

    for (size_t i = 0; i < sizeof read_buff; i++)
        is_empty &= read_buff[i] == '\0';
    U_DEBUG("readline buff is empty? %d\n", is_empty);
    if (is_empty) {
        rd = read(exec_ctx->read_fd, read_buff, sizeof read_buff);
        if (rd < 0)
            return (rd == 0);
    } else
        rd = BULK_READ_BUFF_SZ;
    if (!read_until_line_ending(exec_ctx, &rh, rd))
        return false;
    return append_null_terminator(out);
}
