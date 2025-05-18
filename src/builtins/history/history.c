/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/

#include <stdio.h>

#include "common.h"
#include "exec.h"

int builtins_history(ef_t *ef, char **args __attribute__((unused)))
{
    his_command_t *cmd_history = ef->exec_ctx->history_command;

    for (int i = 0; i < cmd_history->sz; i++){
        if (cmd_history[i].arg) {
            printf("%d\t%s %s\n", i + 1, cmd_history[i].command,
                cmd_history[i].arg);
        } else
            printf("%d\t%s\n", i + 1, cmd_history[i].command);
    }
    return RETURN_SUCCESS;
}
