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

int builtins_exit(env_t *env, char **args __attribute__((unused)), char *buff)
{
    free_env(env);
    free((void *)args);
    free(buff);
    exit(RETURN_SUCCESS);
}

int builtins_cd(env_t *env, char **args, char *buff __attribute__((unused)))
{

}
