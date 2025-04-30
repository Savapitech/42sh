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

const char *OTHER_BUILTINS[] = {
    "echo"
};

const size_t OTHER_BUILTINS_SZ = sizeof OTHER_BUILTINS
/ sizeof *OTHER_BUILTINS;

static
void search_builtins(ef_t *ef, char *arg)
{
    for (size_t i = 0; i < BUILTINS_SZ; i++) {
        if (u_strlen(BUILTINS[i].name) != (int)strlen(arg))
            continue;
        if (u_strcmp(BUILTINS[i].name, arg) == 0)
            dprintf(ef->out_fd, "%s is a shell built-in\n", arg);
    }
    for (size_t i = 0; i < OTHER_BUILTINS_SZ; i++) {
        if (u_strlen(OTHER_BUILTINS[i]) != (int)strlen(arg))
            continue;
        if (u_strcmp(OTHER_BUILTINS[i], arg) == 0)
            dprintf(ef->out_fd, "%s is a shell built-in\n", arg);
    }
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
    parse_alias(&alias_path.str, &alias_path.sz, ef->exec_ctx->alias);
    if (strcmp(arg, alias_path.str) != 0)
        dprintf(ef->out_fd, "%s is aliased to %s\n", arg, alias_path.str);
    search_builtins(ef, arg);
    if (strcmp(arg, bin_path) != 0)
        dprintf(ef->out_fd, "%s\n", bin_path);
    free(alias_path.str);
    return true;
}

int builtins_where(ef_t *ef, char **args)
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
