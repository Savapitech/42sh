/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "env.h"
#include "shell.h"
#include "u_str.h"

int builtins_exit(env_t *env, char **args __attribute__((unused)), char *buff,
    history_t *history)
{
    free_env(env);
    free((void *)args);
    free(buff);
    exit(history->last_exit_code);
}

int builtins_env(env_t *env, char **args __attribute__((unused)),
    char *buff __attribute__((unused)),
    history_t *history __attribute__((unused)))
{
    for (size_t i = 0; i < env->sz; i++) {
        if (env->env[i] == NULL)
            continue;
        write(STDOUT_FILENO, env->env[i], u_strlen(env->env[i]));
        WRITE_CONST(STDOUT_FILENO, "\n");
    }
    return RETURN_SUCCESS;
}

int builtins_setenv(env_t *env, char **args,
    char *buff __attribute__((unused)),
    history_t *history __attribute__((unused)))
{
    if (args[1] == NULL)
        return builtins_env(env, args, buff, history);
    if (args[2] != NULL && args[3] != NULL) {
        WRITE_CONST(STDERR_FILENO, "setenv: Too many arguments.\n");
        return RETURN_FAILURE;
    }
    if (!isalpha(args[1][0]))
        return (WRITE_CONST(STDERR_FILENO, "setenv: Variable name must begin"
            " with a letter.\n"), RETURN_FAILURE);
    if (!u_str_is_only_alnum(args[1]))
        return (WRITE_CONST(STDERR_FILENO, "setenv: Variable name must contain"
            " alphanumeric characters.\n"), RETURN_FAILURE);
    if (!set_env(env, args[1], args[2]))
        return RETURN_FAILURE;
    return RETURN_SUCCESS;
}

int builtins_unsetenv(env_t *env, char **args,
    __attribute__((unused)) char *buff,
    history_t *history __attribute__((unused)))
{
    if (args[1] == NULL)
        return (WRITE_CONST(STDERR_FILENO, "unsetenv: Too few arguments.\n"),
            RETURN_FAILURE);
    if (!unset_env(env, args[1]))
        return RETURN_FAILURE;
    return RETURN_SUCCESS;
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

static
int builtins_cd_chdir(char *path, history_t *history, char **args, env_t *env)
{
    bool last_chdir_set = false;

    if (history->last_chdir != NULL && u_strcmp(args[1], "-") == 0) {
        path = history->last_chdir;
        free(history->last_chdir);
        history->last_chdir = u_strdup(get_env_value(env, "PWD"));
        last_chdir_set = true;
    }
    if (chdir(path) < 0) {
        write(STDERR_FILENO, path, u_strlen(path));
        cd_print_error();
        return RETURN_FAILURE;
    }
    if (!last_chdir_set) {
        free(history->last_chdir);
        history->last_chdir = u_strdup(path);
    }
    return RETURN_SUCCESS;
}

int builtins_cd(env_t *env, char **args, char *buff __attribute__((unused)),
    history_t *history __attribute__((unused)))
{
    char *path = args[1];

    if (path == NULL || u_strcmp(args[1], "~") == 0)
        path = get_env_value(env, "HOME");
    if (path == NULL)
        return RETURN_FAILURE;
    if (args[1] != NULL && args[2] != NULL) {
        WRITE_CONST(STDERR_FILENO, "cd: Too many arguments.\n");
        return RETURN_FAILURE;
    }
    return builtins_cd_chdir(path, history, args, env);
}
