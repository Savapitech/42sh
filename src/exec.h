/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef EXEC_H
    #define EXEC_H
    #include <stdint.h>

    #include "env.h"

typedef struct {
    char const *name;
    void (*ptr)(uint64_t a);
} builtins_t;

typedef struct {
    char **args;
    size_t count;
    size_t cap;
} args_t;

int execute(char *buffer, env_t *env);
#endif /* EXEC_H */
