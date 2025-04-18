/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef ALIAS_H
    #define ALIAS_H
    #include "env.h"
    #include "history.h"
    #include "shell.h"

typedef struct alias_s {
    size_t size;
    char **alias_array;
    char **alias_to_replace;
} alias_t;

void free_alias(alias_t *alias);
int builtins_display_alias(alias_t *alias);
int parse_alias(char **buffer, size_t *buffer_len, alias_t *alias);

#endif /* ALIAS*/
