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
    printf("Alias %d:\n", alias->size);
    for (size_t i = 0; i != alias->size; i++){
        printf("|| Alias: %s || ", alias->alias_array[i]);
        printf("Command: %s ||\n", alias->alias_to_replace[i]);
    }
    return RETURN_SUCCESS;
}

char *array_nto_strdup(char **array, int i)
{
    char *new_str = NULL;
    int size_str = 0;
    int letter = 0;

    if (len_array(array) < i)
        return NULL;
    for (int cp_i = i;array[cp_i] != NULL; cp_i++)
        size_str += strlen(array[cp_i]) + 1;
    new_str = malloc(size_str + 1);
    if (new_str == NULL)
        return NULL;
    for (int j = i; array[j] != NULL; j++){
        for (int k = 0; array[j][k] != '\0'; k++){
            new_str[letter] = array[j][k];
            letter++;
        }
        new_str[letter] = ' ';
        letter++;
    }
    new_str[letter] = '\0';
    return new_str;
}

int builtins_alias(ef_t *ef, char **args)
{
    alias_t *alias = ef->exec_ctx->alias;
    char *first_arg = args[1];

    if (first_arg != NULL && strcmp(args[1], "--display") == 0)
        return builtins_display_alias(alias);
    if (len_array(args) < 3){
        fprintf(stderr, "alias [cpy] [command]\n");
        return RETURN_FAILURE;
    }
    alias->alias_array[alias->size - 1] = strdup(args[1]);
    //for (int i = 2; args[i] != NULL; i++)
        //printf("ALIAS \\;/%p\\\n", args[i]);
    alias->alias_to_replace[alias->size - 1] = array_nto_strdup(args, 2);
    //printf("ALIAS\n");
        //return RETURN_FAILURE;
    return RETURN_SUCCESS;
}