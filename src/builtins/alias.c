/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alias.h"
#include "common.h"
#include "exec.h"
#include "utils.h"

void free_alias(alias_t *alias)
{
    for (size_t i = 0; i < alias->size; i++){
        free(alias->alias_array[i]);
        free(alias->alias_to_replace[i]);
    }
    free(alias->alias_array);
    free(alias->alias_to_replace);
}

int builtins_display_alias(alias_t *alias)
{
    for (size_t i = 0; i != alias->size; i++)
        printf("%s\t%s\n", alias->alias_array[i], alias->alias_to_replace[i]);
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
    char *new_str = nullptr;
    int size_str = 0;
    int letter = 0;

    if (len_array(array) < i)
        return nullptr;
    size_str = size_str_in_narray(i, array);
    new_str = malloc(size_str + 1);
    if (new_str == NULL)
        return nullptr;
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
int add_alias_array(alias_t *alias, char **args)
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
    return RETURN_SUCCESS;
}

int add_alias(alias_t *alias, char **args)
{
    int replace = -1;

    for (size_t i = 0; i != alias->size - 1; i++)
        if (!strcmp(args[1], alias->alias_array[i]))
            replace = (int)(i);
    if (replace == -1)
        return add_alias_array(alias, args);
    alias->size--;
    free(alias->alias_to_replace[replace]);
    alias->alias_to_replace[replace] = array_nto_strdup(args, 2);
    return RETURN_SUCCESS;
}

int builtins_alias(ef_t *ef, char **args)
{
    alias_t *alias = ef->exec_ctx->alias;

    if (args[1] != NULL && strcmp(args[1], "-h") == 0)
        return fprintf(stderr, "alias [cpy] [command]\n"), RETURN_FAILURE;
    if (len_array(args) < 3) {
        builtins_display_alias(alias);
        return RETURN_SUCCESS;
    }
    alias->size++;
    return add_alias(alias, args);
}
