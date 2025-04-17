/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** strn_to_ndup
*/
#include <stdlib.h>

char *strn_to_ndup(int start, int size, char *str)
{
    char *new_str = malloc(sizeof(char) * (size + 1));
    int count = 0;

    if (new_str == NULL)
        return NULL;
    new_str[size] = '\0';
    for (int i = start; i != start + size; i++){
        new_str[count] = str[i];
        count++;
    }
    return new_str;
}
