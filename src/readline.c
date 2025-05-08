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
bool copy_single_char(
    exec_ctx_t *exec_ctx,
    buff_t *out,
    char *cpy,
    char in)
{
    if (in == '\r')
        in = '\n';
    if (in == '\0' || in == '\f')
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
    exec_ctx_t *ec, readline_helper_t *rh, ssize_t rd,
    text_parse_info_t *tpi)
{
    ssize_t i = 0;
    ssize_t skip;

    for (bool done = false; !done && i < rd; i++) {
        if (copy_single_char(ec, rh->out, &rh->cpy[tpi->written], rh->in[i])) {
            done = rh->cpy[tpi->written] == '\n';
            tpi->written++;
            continue;
        }
        skip = handle_keys(ec, rh->out, &rh->in[i], BULK_READ_BUFF_SZ - i);
        if (skip < 0) {
            tpi->used = i;
            return false;
        }
        i += skip - 1;
    }
    if (ec->isatty)
        write(STDOUT_FILENO, rh->cpy, tpi->written);
    tpi->used = i;
    return true;
}

static
bool read_until_line_ending(
    exec_ctx_t *ec, readline_helper_t *rh, ssize_t rd)
{
    text_parse_info_t tpi;

    for (;;) {
        memset(&tpi, '\0', sizeof tpi);
        if (!ensure_buff_av_capacity(rh->out, BULK_READ_BUFF_SZ))
            return false;
        if (!populate_copy_buff(ec, rh, rd, &tpi))
            return true;
        U_DEBUG("copied %zu chars to cpy (%zu used)\n", tpi.written, tpi.used);
        memmove(rh->in, &rh->in[tpi.used], BULK_READ_BUFF_SZ - tpi.used);
        memset(&rh->in[BULK_READ_BUFF_SZ - tpi.used], '\0', tpi.used);
        if (rh->cpy[tpi.written - 1] == '\n')
            break;
        rd = read(ec->read_fd, rh->in, BULK_READ_BUFF_SZ);
        if (rd <= 0)
            return (rd == 0);
        U_DEBUG("read %zu characters\n", rd);
    }
    return true;
}

bool readline(exec_ctx_t *ec, buff_t *out)
{
    static char read_buff[BULK_READ_BUFF_SZ] = {0};
    readline_helper_t rh = { out, read_buff, { 0 } };
    bool is_empty = true;
    ssize_t rd = 0;

    for (size_t i = 0; i < sizeof read_buff; i++)
        is_empty &= read_buff[i] == '\0';
    U_DEBUG("readline buff is empty? %d\n", is_empty);
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
