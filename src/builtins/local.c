/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** local
*/

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "env.h"
#include "exec.h"
#include "u_str.h"
#include "u_mem.h"

bool check_local_var(char *var, char *func_name)
{
    if (!isalpha(var[0]))
        return (fprintf(stdout, "%s: Variable name must begin"
            " with a letter.\n", func_name), RETURN_FAILURE);
    if (!u_str_is_only_alnum(var))
        return (fprintf(stdout, "%s: Variable name must contain"
            " alphanumeric characters.\n", func_name), RETURN_FAILURE);
    return RETURN_SUCCESS;
}

char *get_local_value(local_t *local, char const *key)
{
    int key_len = u_strlen(key);

    for (size_t i = 0; i < local->sz; i++) {
        if (local->local_var[i] == NULL)
            continue;
        if (u_strcspn(local->local_var[i], '\t') != key_len)
            continue;
        if (u_strcmp(local->local_var[i], key) == 0)
            return local->local_var[i] + key_len + 1;
    }
    return NULL;
}

local_t create_local(void)
{
    local_t local = {.sz = 0, .cap = 2};

    local.local_var = (char **)malloc(sizeof(char *) * local.cap);
    if (local.local_var == NULL)
        return (local_t){.sz = 0, .cap = 2, .local_var = NULL};
    local.local_var[local.sz] = NULL;
    local.sz++;
    return local;
}

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

static
void unset_local_move(local_t *local, size_t i)
{
    while (local->local_var[i]) {
        local->local_var[i] = local->local_var[i + 1];
        i++;
    }
}

bool unset_local(local_t *local, char *var)
{
    int key_len = u_strlen(var);

    for (size_t i = 0; i < local->sz; i++) {
        if (local->local_var[i] == NULL)
            continue;
        if (u_strcspn(local->local_var[i], '\t') != key_len)
            continue;
        if (u_strcmp(local->local_var[i], var) == 0) {
            unset_local_move(local, i);
            local->sz--;
            return true;
        }
    }
    return false;
}
