/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#ifndef BUILTINS_HANDLER_H
    #define BUILTINS_HANDLER_H
    #include "env.h"
    #include "history.h"
    #include "shell.h"
    #include "alias.h"

typedef struct {
    env_t *env;
    history_t *history;
    his_command_t *history_command;
    alias_t *alias;
} builtin_handler_t;

size_t update_command(char **buffer,
    size_t *buffer_sz, builtin_handler_t *builtin_handler);
#endif /* BUILTINS_HANDLER_H */
