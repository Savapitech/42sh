/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "alias.h"
#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "path.h"
#include "u_str.h"

const char *OOTHER_BUILTINS[] = {
    "echo"
};

const size_t OOTHER_BUILTINS_SZ = sizeof OOTHER_BUILTINS
/ sizeof *OOTHER_BUILTINS;

static
bool search_builtin(ef_t *ef, char *arg)
{
    for (size_t i = 0; i < BUILTINS_SZ; i++) {
        if (u_strlen(BUILTINS[i].name) != (int)strlen(arg))
            continue;
        if (u_strcmp(BUILTINS[i].name, arg) == 0)
            return dprintf(ef->out_fd, "%s: shell built-in command.\n", arg),
                true;
    }
    for (size_t i = 0; i < OOTHER_BUILTINS_SZ; i++) {
        if (u_strlen(OOTHER_BUILTINS[i]) != (int)strlen(arg))
            continue;
        if (u_strcmp(OOTHER_BUILTINS[i], arg) == 0)
            return dprintf(ef->out_fd, "%s: shell built-in command.\n", arg),
                true;
    }
    return false;
}

static
bool search_cmd(ef_t *ef, char *arg)
{
    char *bin_path = parse_full_bin_path(ef->env, arg);
    buff_t alias_path = { .str = strdup(arg), strlen(arg) };

    if (alias_path.str == NULL)
        return false;
    if (bin_path == NULL)
        return free(alias_path.str), NULL;
    parse_alias(&alias_path.str, ef->exec_ctx->alias);
    if (strcmp(arg, alias_path.str) != 0)
        return dprintf(ef->out_fd, "%s:\t aliased to %s\n", arg,
            alias_path.str), true;
    if (search_builtin(ef, arg))
        return true;
    if (strcmp(arg, bin_path) != 0)
        return dprintf(ef->out_fd, "%s\n", bin_path), true;
    fprintf(stderr, "%s: Command not found.\n", arg);
    free(alias_path.str);
    return true;
}

int builtins_which(ef_t *ef, char **args)
{
    if (args[1] == NULL)
        return dprintf(STDERR_FILENO, "%s: Too few arguments\n", args[0]),
            RETURN_FAILURE;
    for (size_t i = 1; args[i] != NULL; i++) {
        if (!search_cmd(ef, args[i]))
            return RETURN_FAILURE;
    }
    return RETURN_SUCCESS;
}
