/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** unset
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "exec.h"
#include "u_str.h"


int builtins_unset(ef_t *ef, char **args)
{
    if (args[1] == NULL)
        return (WRITE_CONST(STDERR_FILENO, "unset: Too few arguments.\n"),
            RETURN_FAILURE);
    for (int i = 1; args[i]; i++)
        unset_local(ef->exec_ctx->local, args[i]);
    return RETURN_SUCCESS;
}
