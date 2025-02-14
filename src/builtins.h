/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#ifndef BUILTINS_H
    #define BUILTINS_H
    #include "env.h"

__attribute__((nonnull))
int builtins_exit(env_t *env, char **args __attribute__((unused)), char *buff);
__attribute__((nonnull))
int builtins_cd(env_t *env, char **args, char *buff __attribute__((unused)));
__attribute__((nonnull))
int builtins_unsetenv(env_t *env, char **args,
    char *buff __attribute__((unused)));
__attribute__((nonnull))
int builtins_env(env_t *env, char **args __attribute__((unused)),
    char *buff __attribute__((unused)));
__attribute__((nonnull))
int builtins_setenv(env_t *env, char **args,
    char *buff __attribute__((unused)));
#endif /* BUILTIND_H */
