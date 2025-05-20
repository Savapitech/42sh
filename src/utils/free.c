/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>

#include "exec.h"

void free_args(char **args)
{
    for (size_t i = 0; args[i] != nullptr; i++)
        free(args[i]);
    free((void *)args);
}

void free_everything(exec_ctx_t *ec)
{
    free_env(ec->env);
    free_alias(ec->alias);
    for (int i = 0; i < ec->history_command->sz; i++) {
        free(ec->history_command[i].command);
        free(ec->history_command[i].arg);
    }
    free(ec->history_command);
    free(ec->local->local_var);
    free(ec->jobs.jobs);
}
