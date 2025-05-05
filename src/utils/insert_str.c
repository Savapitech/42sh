/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *insert_str(const char *base, const char *insert, size_t pos)
{
    size_t base_len = strlen(base);
    size_t insert_len = strlen(insert);
    size_t new_len;
    char *res;

    if (pos >= base_len)
        pos = base_len;
    new_len = base_len - 1 + insert_len;
    res = malloc(sizeof(char) * (new_len + 1));
    if (res == nullptr)
        return nullptr;
    memcpy(res, base, pos);
    strncpy(res + pos, insert, insert_len);
    memcpy(res + pos + insert_len, base + pos + 1, base_len - pos - 1 + 1);
    return res;
}
