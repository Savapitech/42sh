/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "env.h"
#include "exec.h"

int builtins_exit(ef_t *ef, char **args __attribute__((unused)))
{
    if (ef->flags & F_PIPE &&
        !(ef->out_fd == STDOUT_FILENO || ef->p_i == ef->p_sz - 1))
        return RETURN_SUCCESS;
    if (!(ef->flags & F_EXIT)) {
        ef->flags |= F_EXIT;
        return RETURN_SUCCESS;
    }
    free_env(ef->env);
    free(ef->buffer);
    exit(ef->builtin_handler->history->last_exit_code);
}
