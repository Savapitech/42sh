/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "env.h"
#include "exec.h"
#include "u_str.h"

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
