/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "common.h"
#include "debug.h"
#include "exec.h"
#include "u_str.h"

static
int get_argc(char **args)
{
    int i = 0;

    for (; *args != NULL; args++)
        i++;
    return i;
}

bool if_repl(ef_t *ef, char **args, char *cmd)
{
    if (strcmp("then", cmd) != 0)
        return visitor(cmd, ef->exec_ctx), true;
    return true;
}

int builtins_if(ef_t *ef, char **args)
{
    int result;
    char *cmd;

    if (args[1] == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Too few arguments.\n"),
            RETURN_FAILURE;
    if (args[2] == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Empty if\n"), RETURN_FAILURE;
    cmd = strdup(args[get_argc(args) - 1]);
    if (cmd == NULL)
        return RETURN_FAILURE;
    args[get_argc(args) - 1] = nullptr;
    result = builtins_expr(ef, args);
    U_DEBUG("If expr result [%d]\n", result);
    if (result == -1)
        return RETURN_FAILURE;
    if (result)
        if (!if_repl(ef, args, cmd))
            return free(cmd), RETURN_FAILURE;
    return free(cmd), RETURN_SUCCESS;
}
