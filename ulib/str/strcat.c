/*
** EPITECH PROJECT, 2023
** CPoolDay07
** File description:
** ./u_strcat.c
*/

#include <stdlib.h>

#include "u_str.h"

char *u_strcat(char *dest, char const *src)
{
    int i;
    int count;
    char *res;

    i = 0;
    count = 0;
    res = malloc(sizeof(*res) * (u_strlen(dest) + u_strlen(src) + 1));
    if (res == NULL)
        return NULL;
    while (dest[i]) {
        res[i] = dest[i];
        i++;
    }
    while (src[count]) {
        res[i + count] = src[count];
        count++;
    }
    res[i + count] = '\0';
    return (res);
}
