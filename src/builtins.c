/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>

#include "common.h"
#include "env.h"
#include "exec.h"

int builtins_exit(env_t *env, char **args __attribute__((unused)), char *buff)
{
    free_env(env);
    free((void *)args);
    free(buff);
    exit(RETURN_SUCCESS);
}
