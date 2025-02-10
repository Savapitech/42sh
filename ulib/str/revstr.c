/*
** EPITECH PROJECT, 2024
** B-CPE-100-REN-1-1-cpoolday06-savinien.petitjean
** File description:
** Task 3
*/

#include "u_str.h"

char *u_revstr(char *str)
{
    int len = u_strlen(str);
    char copy;

    for (int i = 0; i < len / 2; i++) {
        copy = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = copy;
    }
    return (str);
}
