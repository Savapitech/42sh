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
    return;
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

static
int size_str_in_narray(int i, char **array)
{
    int size_str = 0;

    for (int cp_i = i; array[cp_i] != NULL; cp_i++)
        size_str += strlen(array[cp_i]) + 1;
    return size_str;
}

static
char *array_nto_strdup(char **array, int i)
{
    char *new_str = NULL;
    int size_str = 0;
    int letter = 0;

    if (len_array(array) < i)
        return NULL;
    size_str = size_str_in_narray(i, array);
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

static
int add_alias(alias_t *alias, char **args)
{
    char **new_alias_array =
        realloc(alias->alias_array, sizeof(char *) * alias->size);
    char **new_replace =
        realloc(alias->alias_to_replace, sizeof(char *) * alias->size);

    if (!new_alias_array || !new_replace){
        free(new_alias_array);
        free(new_replace);
        return RETURN_FAILURE;
    }
    alias->alias_array = new_alias_array;
    alias->alias_to_replace = new_replace;
    alias->alias_array[alias->size - 1] = strdup(args[1]);
    alias->alias_to_replace[alias->size - 1] = array_nto_strdup(args, 2);
    return 0;
}

int builtins_alias(ef_t *ef, char **args)
{
    alias_t *alias = ef->exec_ctx->alias;
    char *first_arg = args[1];

    if (first_arg != NULL && strcmp(args[1], "-h") == 0){
        fprintf(stderr, "alias [cpy] [command]\n");
        return RETURN_FAILURE;
    }
    if (len_array(args) < 3){
        builtins_display_alias(alias);
        return RETURN_SUCCESS;
    }
    alias->size++;
    add_alias(alias, args);
    return RETURN_SUCCESS;
}
