/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>
#include <unistd.h>

#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "u_str.h"

int builtins_break(ef_t *ef, char **args)
{
    if (args[1] != NULL)
        WRITE_CONST(STDERR_FILENO, "break: Too many arguments.\n");
    if (ef->env->in_loop == false && args[1] == NULL)
        return (WRITE_CONST(STDERR_FILENO, "break: Not in while/foreach.\n"),
            RETURN_FAILURE);
    if (ef->env->in_loop == true){
        free_env(ef->env);
        free(ef->buffer);
        exit(RETURN_SUCCESS);
    }
    return RETURN_FAILURE;
}

int builtins_builtins(ef_t *ef, char **args[[gnu::unused]])
{
    for (size_t i = 0; i < BUILTINS_SZ; i++) {
        write(ef->out_fd, BUILTINS[i].name, u_strlen(BUILTINS[i].name));
        WRITE_CONST(ef->out_fd, "\n");
    }
    return RETURN_SUCCESS;
}
