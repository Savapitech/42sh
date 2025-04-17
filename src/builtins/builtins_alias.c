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
    alias_t *alias = ef->exec_ctx->alias;
    char *first_arg = args[1];

    if (first_arg != NULL && strcmp(args[1], "--display") == 0)
        return builtins_display_alias(alias);
    if (len_array(args) != 3){
        fprintf(stderr, "alias [cpy] [command]\n");
        return RETURN_FAILURE;
    }
    alias->alias_array[alias->size - 1] = strdup(args[1]);
    alias->alias_to_replace[alias->size - 1] = strdup(args[2]);
    printf("ALIAS\n");
        //return RETURN_FAILURE;
    return RETURN_SUCCESS;
}