/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** change_local
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

bool check_var(char *local, char *var)
{
    if (!strncmp(local, var, strlen(var)))
        return true;
    return false;
}

char *reset_value(char *value, char *var)
{
    char *new_local = malloc(sizeof(char) * (u_strlen(value) + u_strlen(var) +
    2));

    if (new_local == NULL)
        return NULL;
    strcpy(new_local, var);
    strcat(new_local, "\t");
    if (value != NULL)
        strcat(new_local, value);
    return new_local;
}

bool change_local(local_t *local, char *var, char *value)
{
    bool find_a_var = false;

    for (int i = 0; local->local_var[i]; i++){
        if (!check_var(local->local_var[i], var))
            continue;
        free(local->local_var[i]);
        local->local_var[i] = reset_value(value, var);
        if (local->local_var[i] == NULL)
            return false;
        find_a_var = true;
        break;
    }
    if (!find_a_var && !set_local(local, var, value))
        return false;
    return true;
}
