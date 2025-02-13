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
#include "u_str.h"

int builtins_exit(env_t *env, char **args __attribute__((unused)), char *buff)
{
    free_env(env);
    free((void *)args);
    free(buff);
    exit(RETURN_SUCCESS);
}

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

int builtins_cd(env_t *env, char **args, char *buff __attribute__((unused)))
{
    char *path = args[1];
    int result = RETURN_SUCCESS;

    if (path == NULL)
        path = get_env_value(env, "HOME");
    if (path == NULL)
        return RETURN_FAILURE;
    if (chdir(path) < 0) {
        write(STDERR_FILENO, path, u_strlen(path));
        cd_print_error();
        result = RETURN_FAILURE;
    }
    return result;
}
