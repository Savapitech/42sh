/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** get_local
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
