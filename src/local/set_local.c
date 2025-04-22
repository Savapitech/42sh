/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** set_local
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "u_mem.h"
#include "u_str.h"
#include "local.h"

static
bool ensure_local_capacity(local_t *local)
{
    char **new_ptr = NULL;

    if (local->sz < local->cap)
        return true;
    new_ptr = (char **)u_realloc((void *)local->local_var,
        sizeof *local->local_var * local->sz,
        sizeof *local->local_var * local->cap << 1);
    if (!new_ptr)
        return false;
    local->cap <<= 1;
    local->local_var = new_ptr;
    return true;
}

bool set_local(local_t *local, char *var, char *value)
{
    char *new_loc = NULL;
    size_t key_len = u_strlen(var);
    size_t value_len = u_strlen(value);

    if (get_local_value(local, var) != NULL)
        unset_local(local, var);
    local->sz++;
    if (!ensure_local_capacity(local))
        return false;
    new_loc = malloc(sizeof(char) * (key_len + value_len + 2));
    if (new_loc == NULL)
        return false;
    u_bzero(new_loc, key_len + value_len + 2);
    u_strcpy(new_loc, var);
    new_loc[key_len] = '\t';
    if (value_len > 0)
        u_strcpy(new_loc + key_len + 1, value);
    local->local_var[local->sz - 1] = new_loc;
    return true;
}
