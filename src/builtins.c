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
#include "exec.h"
#include "u_mem.h"
#include "u_str.h"

int builtins_exit(ef_t *ef, char **args __attribute__((unused)))
{
    if (ef->flags & F_PIPE &&
        !(ef->out_fd == STDOUT_FILENO || ef->p_i == ef->p_sz - 1))
        return RETURN_SUCCESS;
    if (!(ef->flags & F_EXIT)) {
        ef->flags |= F_EXIT;
        return RETURN_SUCCESS;
    }
    free_env(ef->env);
    free(ef->buffer);
    WRITE_CONST(STDOUT_FILENO, "exit\n");
    exit(ef->history->last_exit_code);
}

int builtins_env(ef_t *ef, char **args __attribute__((unused)))
{
    for (size_t i = 0; i < ef->env->sz; i++) {
        if (ef->env->env[i] == NULL)
            continue;
        write(ef->out_fd, ef->env->env[i], u_strlen(ef->env->env[i]));
        WRITE_CONST(ef->out_fd, "\n");
    }
    return RETURN_SUCCESS;
}

int builtins_setenv(ef_t *ef, char **args)
{
    if (args[1] == NULL)
        return builtins_env(ef, args);
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
    if (!set_env(ef->env, args[1], args[2]))
        return RETURN_FAILURE;
    return RETURN_SUCCESS;
}

int builtins_unsetenv(ef_t *ef, char **args)
{
    if (args[1] == NULL)
        return (WRITE_CONST(STDERR_FILENO, "unsetenv: Too few arguments.\n"),
            RETURN_FAILURE);
    if (!unset_env(ef->env, args[1]))
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

    if (ef->history->last_chdir != NULL && args[1] != NULL
        && u_strcmp(args[1], "-") == 0)
        path = ef->history->last_chdir;
    act_pwd = get_current_dir();
    if (chdir(path) < 0) {
        write(STDERR_FILENO, path, u_strlen(path));
        cd_print_error();
        return RETURN_FAILURE;
    }
    free(ef->history->last_chdir);
    ef->history->last_chdir = act_pwd;
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
