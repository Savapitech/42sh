/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#ifndef UTILS_H
    #define UTILS_H
    #include "history.h"
    #include "u_str.h"

char *strn_to_ndup(int start, int size, char *str);
bool is_a_token(char *str, int index_str);
char *cat_in_str(his_variable_t *his_variable, char *str, char *cpy);
#endif /* UTILS_H */
