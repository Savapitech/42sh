/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>

#include "exec.h"


int builtins_termname(ef_t *ef, char **)
{
    char *termname = get_env_value(ef->env, "TERM");

    if (termname == nullptr)
        return RETURN_FAILURE;
    printf("%s\n", termname);
    return RETURN_SUCCESS;
}
