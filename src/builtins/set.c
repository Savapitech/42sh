/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** set
*/

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "env.h"
#include "exec.h"
#include "u_str.h"

void print_local(ef_t *ef)
{
    for (size_t i = 0; i < ef->exec_ctx->local->sz; i++) {
        if (ef->exec_ctx->local->local_var[i] == NULL)
            continue;
        write(ef->out_fd, ef->exec_ctx->local->local_var[i],
            u_strlen(ef->exec_ctx->local->local_var[i]));
        WRITE_CONST(ef->out_fd, "\n");
    }
}

int builtins_set(ef_t *ef, char **args)
{
    char *var = NULL;

    if (args[1] == NULL)
        return (print_local(ef), RETURN_SUCCESS);
    for (int i = 1; args[i]; i++){
        if (check_local_var(args[i], args[0]))
            return RETURN_FAILURE;
        var = args[i];
        i++;
        if (!args[i])
            return (set_local(ef->exec_ctx->local, var, NULL)
            , RETURN_SUCCESS);
        if (strcmp(args[i], "="))
            return RETURN_FAILURE;
        i++;
        if (!set_local(ef->exec_ctx->local, var, args[i]))
            return RETURN_FAILURE;
    }
    return RETURN_SUCCESS;
}
