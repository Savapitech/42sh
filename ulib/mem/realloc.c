/*
** EPITECH PROJECT, 2024
** CPoolDay08
** File description:
** ./u_realloc.c
*/

#include <stddef.h>
#include <stdlib.h>

#include "u_mem.h"

void *u_realloc(void *ptr, size_t actual_size, size_t new_size)
{
    void *new;

    if (ptr == NULL)
        return ptr;
    if (!actual_size && !new_size)
        return NULL;
    new = malloc(new_size);
    if (actual_size > 0 && new_size > 0)
        u_memcpy(new, ptr, actual_size);
    free(ptr);
    return new;
}
