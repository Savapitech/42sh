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
    char *new_var = NULL;

    local->sz++;
    if (!ensure_local_capacity(local))
        return false;
    new_var = malloc(sizeof(char) * (u_strlen(var) + u_strlen(value) + 2));
    if (new_var == NULL)
        return false;
    strcpy(new_var, var);
    strcat(new_var, "\t");
    if (value != NULL)
        strcat(new_var, value);
    local->local_var[local->sz - 1] = new_var;
    return true;
}
