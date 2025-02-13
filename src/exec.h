/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef EXEC_H
    #define EXEC_H
    #include "env.h"
    #define DEFAULT_ARGS_CAP 1

typedef struct {
    char const *name;
    int (*ptr)(env_t *env, char **args, char *buff);
} builtins_t;

typedef struct {
    char **args;
    size_t count;
    size_t cap;
} args_t;

int execute(char *buffer, env_t *env);
void free_args(char **args);
#endif /* EXEC_H */
