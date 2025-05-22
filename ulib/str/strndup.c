/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>
#include <string.h>

char *u_strndup(char const *src, size_t n)
{
    char *dst = malloc((sizeof *src * n) + 1);

    if (dst == nullptr)
        return nullptr;
    dst = strncpy(dst, src, n);
    dst[n] = '\0';
    return dst;
}
