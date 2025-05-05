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
    char **dup_array = nullptr;
    int i = 0;

    if (array == nullptr)
        return nullptr;
    dup_array = (char **)malloc(sizeof(char *) * (my_array_len(array) + 1));
    if (dup_array == nullptr)
        return nullptr;
    for (; array[i] != nullptr; i++)
        dup_array[i] = u_strdup(array[i]);
    dup_array[i] = nullptr;
    return dup_array;
}
