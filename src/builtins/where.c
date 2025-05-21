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
#include "u_str.h"

const char *OTHER_BUILTINS[] = {
    "pouet"
};

const size_t OTHER_BUILTINS_SZ = sizeof OTHER_BUILTINS
/ sizeof *OTHER_BUILTINS;

static
void search_builtins(ef_t *ef, char *arg)
{
    for (size_t i = 0; i < BUILTINS_SZ; i++) {
        if (u_strcmp(BUILTINS[i].name, arg) == 0)
            dprintf(ef->out_fd, "%s is a shell built-in\n", arg);
    }
    for (size_t i = 0; i < OTHER_BUILTINS_SZ; i++) {
        if (u_strcmp(OTHER_BUILTINS[i], arg) == 0)
            dprintf(ef->out_fd, "%s is a shell built-in\n", arg);
    }
}

static
void search_all_paths(ef_t *ef, char *arg)
{
    char *path = get_env_value(ef->env, "PATH");
    char *copy = path ? strdup(path) : nullptr;
    char *saveptr = nullptr;
    size_t len;
    char *full;

    if (!copy)
        return;
    for (char *dir = strtok_r(copy, ":", &saveptr); dir != NULL;
            dir = strtok_r(nullptr, ":", &saveptr)) {
        len = strlen(dir) + strlen(arg) + 2;
        full = malloc(len);
        if (!full)
            continue;
        snprintf(full, len, "%s/%s", dir, arg);
        if (access(full, X_OK) == 0)
            dprintf(ef->out_fd, "%s\n", full);
        free(full);
    }
    free(copy);
}

static
bool search_cmd(ef_t *ef, char *arg)
{
    buff_t alias_path = { .str = strdup(arg), .sz = strlen(arg) };

    if (!alias_path.str)
        return false;
    parse_alias(&alias_path.str, ef->exec_ctx->alias);
    if (strcmp(arg, alias_path.str) != 0)
        dprintf(ef->out_fd, "%s is aliased to %s\n", arg, alias_path.str);
    search_builtins(ef, arg);
    search_all_paths(ef, arg);
    free(alias_path.str);
    return true;
}

int builtins_where(ef_t *ef, char **args)
{
    if (!args[1])
        return dprintf(STDERR_FILENO, "%s: Too few arguments\n", args[0]),
            RETURN_FAILURE;
    for (size_t i = 1; args[i]; i++) {
        if (!search_cmd(ef, args[i]))
            return RETURN_FAILURE;
    }
    return RETURN_SUCCESS;
}
