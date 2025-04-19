/*
** EPITECH PROJECT, 2025
** minish
** File description:
** arraydup
*/
#include "u_str.h"
#include <stdlib.h>

char **arraydup(char **array)
{
    char **dup_array = NULL;
    int i = 0;

    if (array == NULL)
        return NULL;
    dup_array = malloc(sizeof(char *) * (my_array_len(array) + 1));
    if (dup_array == NULL)
        return NULL;
    for (; array[i] != NULL; i++)
        dup_array[i] = u_strdup(array[i]);
    dup_array[i] = NULL;
    return dup_array;
}
