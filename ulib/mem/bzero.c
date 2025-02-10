/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdbool.h>
#include <stddef.h>

__attribute__((nonnull(1)))
bool u_bzero(char *restrict str, size_t sz)
{
    if (sz > 0)
        for (size_t i = 0; i < sz; i++)
            str[i] = 0;
    return true;
}
