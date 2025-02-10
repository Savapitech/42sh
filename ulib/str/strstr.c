/*
** EPITECH PROJECT, 2024
** B-CPE-100-REN-1-1-cpoolday06-savinien.petitjean
** File description:
** Task 2
*/

#include <stddef.h>

#include "u_str.h"

char *u_strstr(char *str, char const *to_find)
{
    int find_len = u_strlen(to_find);

    if (*to_find == '\0')
        return str;
    for (; *str != '\0'; str++)
        if (!u_strncmp(str, to_find, find_len))
            return str;
    return NULL;
}
