/*
** EPITECH PROJECT, 2024
** CPoolDay08
** File description:
** ./u_strdup.c
*/

#include <stdlib.h>

#include "u_mem.h"
#include "u_str.h"

char *u_strdup(char const *src)
{
    char *dest;
    int len = u_strlen(src);

    dest = malloc(sizeof(char) * (len + 2));
    if (dest == NULL)
        return NULL;
    u_bzero(dest, len + 2);
    return u_strcpy(dest, src);
}
