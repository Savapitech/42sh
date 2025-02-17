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

int builtins_builtins(env_t *env __attribute__((unused)),
    char **args __attribute__((unused)), char *buff __attribute__((unused)),
    history_t *history __attribute__((unused)))
{
    for (size_t i = 0; i < BUILTINS_SZ; i++)
        write(STDOUT_FILENO, BUILTINS[i].name, u_strlen(BUILTINS[i].name));
    return RETURN_SUCCESS;
}
