/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** u_strnchr
*/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

char *u_strnchr(char *str, char to_search, size_t n)
{
    for (size_t i = 0; str[i] && i < n; i++)
        if (str[i] == to_search)
            return &str[i];
    return NULL;
}
