/*
** EPITECH PROJECT, 2024
** CPoolDay08
** File description:
** ./u_realloc.c
*/

#include <stddef.h>

void *u_memcpy(char *dst, char const *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++)
        dst[i] = src[i];
    return dst;
}
