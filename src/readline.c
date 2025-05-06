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

#include "common.h"
#include "debug.h"
#include "readline.h"
#include "repl.h"
#include "u_str.h"

static
bool str_printable(char const *str, size_t size)
{
    for (size_t i = 0; i < size; i++)
        if (!isprint(str[i]))
            return false;
    return true;
}

static
bool ensure_buff_av_capacity(buff_t *buff, size_t requested)
{
    char *new_str;
    size_t endsize = BUFF_INIT_SZ;

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
bool ensure_buff_capacity(buff_t *buff)
{
    char *new_str;

    if (buff->str == NULL) {
        new_str = malloc((sizeof *buff->str) * BUFF_INIT_SZ);
        if (new_str == NULL)
            return false;
        buff->str = new_str;
        buff->cap = BUFF_INIT_SZ;
    }
    if (buff->sz == buff->cap) {
        new_str = realloc(buff->str, (sizeof *buff->str) * buff->cap << 1);
        if (new_str == NULL)
            return false;
        buff->str = new_str;
        buff->cap <<= 1;
    }
    return true;
}

static
bool append_null_terminator(buff_t *buff, exec_ctx_t *exec_ctx)
{
    if (!ensure_buff_av_capacity(buff, 1))
        return false;
    buff->str[buff->sz - 1] = '\0';
    buff->sz++;
    if (isatty(exec_ctx->read_fd))
        WRITE_CONST(STDOUT_FILENO, "\n");
    U_DEBUG("Buffer [%s]\n", buff->str);
    return true;
}

static
int8_t handle_line_buff(exec_ctx_t *exec_ctx, buff_t *buff, char *read_buff,
    ssize_t read_size)
{
    if (handle_keys(exec_ctx, buff, read_buff))
        return RETURN_SUCCESS;
    if (isatty(exec_ctx->read_fd) && str_printable(read_buff, read_size))
        write(STDOUT_FILENO, read_buff, read_size);
    if (!ensure_buff_av_capacity(buff, read_size))
        return RETURN_FAILURE;
    strncpy(buff->str + buff->sz,
        read_buff, read_size);
    buff->sz += read_size;
    return -1;
}

bool readline(exec_ctx_t *exec_ctx, buff_t *buff, int fd)
{
    char read_buff[2] = "";
    ssize_t read_size = 0;

    if (!ensure_buff_capacity(buff))
        return false;
    while (strchr(read_buff, '\n') == NULL && *read_buff != '\r') {
        memset(read_buff, '\0', sizeof read_buff);
        read_size = read(fd, &read_buff, sizeof read_buff - 1);
        if (read_size < 0)
            return false;
        if (read_size == 0)
            return true;
        if (handle_line_buff(exec_ctx, buff, read_buff, read_size) > -1)
            return true;
    }
    return append_null_terminator(buff, exec_ctx);
}
