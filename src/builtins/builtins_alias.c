/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "env.h"
#include "exec.h"
#include "alias.h"

int builtins_display_alias(alias_t *alias)
{
    printf("DISPLAY ALIAS\n");
    return RETURN_SUCCESS;
}

int builtins_alias(ef_t *ef, char **args)
{
    alias_t alias; //= ef->builtin_handler->history_command;
    char *first_arg = args[1];

    alias.size = 0;
    alias.alias_array = NULL;
    if (first_arg != NULL && strcmp(args[1], "--display") == 0)
        return builtins_display_alias(&alias);

    printf("ARG 1: %s\n", args[1]);
    printf("J ACTIVE LA SAUVEGARDE DE L ALIAS\n");
    return RETURN_SUCCESS;
}

