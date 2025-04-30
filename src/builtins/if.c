/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <unistd.h>

#include "builtins.h"
#include "common.h"
#include "debug.h"
#include "exec.h"
#include "u_str.h"

int builtins_if(ef_t *ef, char **args)
{
    int result;

    if (args[1] == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Too few arguments.\n"),
            RETURN_FAILURE;
    if (args[2] == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Empty if\n"), RETURN_FAILURE;
    result = builtins_expr(ef, args);
    U_DEBUG("If expr result [%d]\n", result);
    if (result) {
        visitor(args[2], ef->exec_ctx);
    }
    return RETURN_SUCCESS;
}
