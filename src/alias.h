/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef ALIAS_H
    #define ALIAS_H
    #include <stddef.h>

typedef struct alias_s {
    size_t size;
    char **alias_array;
    char **alias_to_replace;
} alias_t;

void free_alias(alias_t *alias);
bool replace_alias(char **buffer, alias_t *alias);
alias_t init_alias(void);
#endif /* ALIAS*/
