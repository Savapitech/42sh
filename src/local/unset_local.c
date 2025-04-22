/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** unset_local
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

local_t *unset_local(local_t *local, char *var, char *value)
{
    for (int i = 0; local->local_var[i]; i++){
        if (!check_var(local->local_var[i], var))
            continue;
        free(local->local_var[i]);
        for (int j = i + 1; local->local_var[j]; j++)
            local->local_var[j - 1] = local->local_var[j];
        break;
    }
    return local;
}
