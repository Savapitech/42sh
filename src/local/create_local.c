/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** create_local
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
