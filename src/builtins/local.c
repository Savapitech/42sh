/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** local
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "u_mem.h"
#include "u_str.h"

bool check_local_var(char *var, char *func_name)
{
    if (!isalpha(var[0]))
        return (fprintf(stderr, "%s: Variable name must begin"
            " with a letter.\n", func_name), RETURN_FAILURE);
    if (!u_str_is_only_alnum(var))
        return (fprintf(stderr, "%s: Variable name must contain"
            " alphanumeric characters.\n", func_name), RETURN_FAILURE);
    return RETURN_SUCCESS;
}

char *get_local_value(local_t *local, char const *key)
{
    int key_len = u_strlen(key);

    for (size_t i = 0; i < local->sz; i++) {
        if (local->local_var[i] == nullptr)
            continue;
        if (u_strcspn(local->local_var[i], '\t') != key_len)
            continue;
        if (u_strcmp(local->local_var[i], key) == 0)
            return local->local_var[i] + key_len + 1;
    }
    return nullptr;
}

local_t create_local(void)
{
    local_t local = {.sz = 0, .cap = 2};

    local.local_var = (char **)malloc(sizeof(char *) * local.cap);
    if (local.local_var == nullptr)
        return (local_t){.sz = 0, .cap = 2, .local_var = nullptr};
    local.local_var[local.sz] = nullptr;
    local.sz++;
    return local;
}

static
bool ensure_local_capacity(local_t *local)
{
    char **new_ptr = nullptr;

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
    char *new_loc = nullptr;
    size_t key_len = u_strlen(var);
    size_t value_len = u_strlen(value);

    if (get_local_value(local, var) != nullptr)
        unset_local(local, var);
    local->sz++;
    if (!ensure_local_capacity(local))
        return false;
    new_loc = malloc(sizeof(char) * (key_len + value_len + 2));
    if (new_loc == nullptr)
        return false;
    u_bzero(new_loc, key_len + value_len + 2);
    u_strcpy(new_loc, var);
    new_loc[key_len] = '\t';
    if (value_len > 0)
        u_strcpy(new_loc + key_len + 1, value);
    local->local_var[local->sz - 1] = new_loc;
    local->local_var[local->sz] = nullptr;
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
        if (local->local_var[i] == nullptr)
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
