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
    F_RED_APPEND = 1 << 2
};

typedef struct {
    char *buffer;
    env_t *env;
    history_t *history;
    ast_ctx_t *ctx;
    ast_t *act_node;
    uint8_t flags;
    char *red_file;
    int red_in_fd;
    int red_out_fd;
    int pipes[2];
    int pin_fd;
    int pout_fd;
} ef_t;

__attribute__((nonnull))
int execute(ef_t *ef);
#endif /* EXEC_H */
