/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** is_a_token
*/

#include "string.h"
#include "ast.h"

bool is_a_token(char *str, int index_str)
{
    str += index_str;
    for (size_t i = 0; i < 16; i++) {
        if (strncmp(str, TOKENS_LIST[i].str, TOKENS_LIST[i].sz) == 0){
            return true;
        }
    }
    return false;
}
