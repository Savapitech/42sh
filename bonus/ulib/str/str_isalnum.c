/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <ctype.h>
#include <stdbool.h>

bool u_str_is_alnum(char *str)
{
    for (; *str != '\0'; str++)
        if (isalnum(*str))
            return true;
    return false;
}

bool u_str_is_only_alnum(char *str)
{
    for (; *str != '\0'; str++)
        if (!isalnum(*str))
            return false;
    return true;
}
