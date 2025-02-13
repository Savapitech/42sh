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
#endif /* BUILTIND_H */
