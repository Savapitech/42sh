/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** cat_in_str
*/
#include "history.h"
#include <string.h>
#include <stdlib.h>

char *cat_in_str(his_variable_t *his_variable, char *str, char *cpy)
{
    int i = 0;
    int len_str = strlen(str);
    int size_right = len_str -
        his_variable->coord_variable - his_variable->size_variable;
    int size_left = (len_str - size_right) - his_variable->size_variable;
    char *new_str = malloc(sizeof(char) *
        (size_right + size_left + strlen(cpy) + 2));

    if (new_str == NULL)
        return NULL;
    for (; i < size_left; i++)
        new_str[i] = str[i];
    for (int j = 0; cpy[j] != 0; j++){
        new_str[i] = cpy[j];
        i++;
    }
    for (int k = 0; k < size_right; k++){
        new_str[i] = str[k + size_left + his_variable->size_variable];
        i++;
    }
    new_str[i] = '\0';
    new_str[i + 1] = '\0';
    return new_str;
}
