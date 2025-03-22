/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <unistd.h>

#include "builtins.h"
#include "common.h"
#include "env.h"
#include "shell.h"
#include "u_str.h"

int builtins_builtins(ef_t *ef, char **args)
{
    for (size_t i = 0; i < BUILTINS_SZ; i++)
        write(ef->out_fd, BUILTINS[i].name, u_strlen(BUILTINS[i].name));
    return RETURN_SUCCESS;
}

int builtins_funny_double_dot(ef_t *ef, char **args)
{
    return RETURN_SUCCESS;
}
