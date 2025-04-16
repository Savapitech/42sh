/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** len_array
*/

#include <stdlib.h>

int len_array(char **array)
{
    int i = 0;

    while(array[i] != NULL)
        i++;
    return i;
}