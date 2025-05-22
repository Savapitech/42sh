/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>

#include "u_str.h"
#include "readline.h"

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
