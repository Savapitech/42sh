/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "env.h"
#include "exec.h"
#include "u_mem.h"
#include "u_str.h"


static
void cd_print_error(void)
{
    switch (errno) {
        case EACCES:
            WRITE_CONST(STDERR_FILENO, ": Permission denied.\n");
            break;
        case ENOENT:
            WRITE_CONST(STDERR_FILENO, ": No such file or directory.\n");
            break;
        case ENOTDIR:
            WRITE_CONST(STDERR_FILENO, ": Not a directory.\n");
            break;
        default:
            WRITE_CONST(STDERR_FILENO, ": Unknown error.\n");
    }
}

static
char *get_current_dir(void)
{
    size_t size = 64;
    char *buffer = malloc(size);
    char *new_buffer;
    size_t max_it = 100;

    if (!buffer)
        return NULL;
    while (getcwd(buffer, size) == NULL && max_it > 0) {
        if (errno != ERANGE)
            return (free(buffer), NULL);
        size <<= 1;
        new_buffer = u_realloc(buffer, u_strlen(buffer) + 1, size);
        if (!new_buffer)
            return (free(buffer), NULL);
        buffer = new_buffer;
        max_it--;
    }
    return buffer;
}

static
int builtins_cd_chdir(ef_t *ef, char **args, char *path)
{
    char *act_pwd;

    if (ef->builtin_handler->history->last_chdir != NULL && args[1] != NULL
        && u_strcmp(args[1], "-") == 0)
        path = ef->builtin_handler->history->last_chdir;
    act_pwd = get_current_dir();
    if (chdir(path) < 0) {
        write(STDERR_FILENO, path, u_strlen(path));
        cd_print_error();
        return RETURN_FAILURE;
    }
    free(ef->builtin_handler->history->last_chdir);
    ef->builtin_handler->history->last_chdir = act_pwd;
    return RETURN_SUCCESS;
}

int builtins_cd(ef_t *ef, char **args)
{
    char *path = args[1];

    if (!(ef->out_fd == STDOUT_FILENO || ef->p_i == ef->p_sz - 1))
        return RETURN_SUCCESS;
    if (path == NULL || u_strcmp(args[1], "~") == 0)
        path = get_env_value(ef->env, "HOME");
    if (path == NULL)
        return RETURN_FAILURE;
    if (args[1] != NULL && args[2] != NULL) {
        WRITE_CONST(STDERR_FILENO, "cd: Too many arguments.\n");
        return RETURN_FAILURE;
    }
    return builtins_cd_chdir(ef, args, path);
}
