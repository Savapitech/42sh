/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef EXEC_H
    #define EXEC_H
    #include "ast.h"
    #include "env.h"
    #include "shell.h"

    #define DEFAULT_ARGS_CAP 1

typedef struct {
    char **args;
    size_t count;
    size_t cap;
} args_t;

enum flags {
    F_PIPE = 1 << 0,
    F_RED = 1 << 1,
    F_EXIT = 1 << 2
};

typedef struct {
    char *buffer;
    env_t *env;
    history_t *history;
    ast_ctx_t *ctx;
    ast_t *act_node;
    size_t skip_i;
    size_t skip_sz;
    uint8_t flags;
    size_t p_i;
    size_t p_sz;
    int rin_fd;
    int rout_fd;
    int pipes[2];
    int pin_fd;
    int pout_fd;
    int in_fd;
    int out_fd;
} ef_t;

__attribute__((nonnull))
int execute(ef_t *ef);
#endif /* EXEC_H */
