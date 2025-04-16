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
#include <string.h>
#include "utils.h"
#include <stdlib.h>

void free_alias(alias_t *alias)
{
    for (size_t i = 0; i != alias->size; i++){
        free(alias->alias_array[i]);
        free(alias->alias_to_replace[i]);
    }
    free(alias->alias_array);
    free(alias->alias_to_replace);
    return ;
}

int builtins_display_alias(alias_t *alias)
{
    for (size_t i = 0; i != alias->size; i++){
        printf("|| Alias: %s || ", alias->alias_array[i]);
        printf("Command: %s ||\n", alias->alias_to_replace[i]);
    }
    return RETURN_SUCCESS;
}

int builtins_alias(ef_t *ef, char **args)
{
    alias_t *alias = ef->builtin_handler->alias;
    char *first_arg = args[1];

    if (first_arg != NULL && strcmp(args[1], "--display") == 0)
        return builtins_display_alias(alias);
    if (len_array(args) != 3)
        return RETURN_FAILURE;
    alias->alias_array[alias->size - 1] = strdup(args[1]);
    alias->alias_to_replace[alias->size - 1] = strdup(args[2]);
    printf("size alias %d\n",len_array(args));// alias->size);
    printf("J ACTIVE LA SAUVEGARDE DE L ALIAS\n");
        //return RETURN_FAILURE;
    return RETURN_SUCCESS;
}

