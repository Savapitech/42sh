/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** break
*/
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "u_str.h"

bool checking_error(ef_t *ef, char **args)
{
    if (args[1])
        return (WRITE_CONST(STDERR_FILENO, "break: Too many arguments.\n"),
        RETURN_FAILURE);
    if (!ef->exec_ctx->local->in_a_loop)
        return (WRITE_CONST(STDERR_FILENO, "break: Not in while/foreach.\n"),
        RETURN_FAILURE);
    return RETURN_SUCCESS;
}

int builtins_break(ef_t *ef, char **args)
{
    if (ef->exec_ctx->local->in_a_loop)
        exit(checking_error(ef, args));
    return checking_error(ef, args);
}
