/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** loop
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "redirects.h"
#include "u_str.h"
#include "u_mem.h"

//TODO: need to know how to get the next arg
ast_t *parse_loop(ast_ctx_t *ctx)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return NULL;
    node->type = N_LOP;
    node->tok = ctx->act_tok;
    node->loop.cap = 2;
    node->loop.buffers = NULL;
    node->loop.sz = 0;
    node->loop.condition = NULL;
    return node;
}

static
void exit_child(int sig __attribute__((unused)))
{
    _exit(sig);
}

static
int while_loop(ef_t *ef, ast_t *node, char **save_buffers)
{
    int status = 0;

    ef->env->in_loop = true;
    for (size_t i = 0; node->loop.buffers[i]; i++){
        status = visitor(node->loop.buffers[i], ef->env, ef->history);
        if (node->loop.buffers == NULL)
            exit(84);
    }
    free_array(node->loop.buffers);
    node->loop.buffers = arraydup(save_buffers);
    ef->env->in_loop = false;
    return status;
}

static
int foreach_loop(ef_t *ef, ast_t *node, char **save_buffers)
{
    int status = 0;

    ef->env->in_loop = true;
    for (size_t i = 0; node->loop.buffers[i]; i++){
        status = visitor(node->loop.buffers[i], ef->env, ef->history);
        if (node->loop.buffers == NULL)
            exit(84);
    }
    free_array(node->loop.buffers);
    node->loop.buffers = arraydup(save_buffers);
    printf("%s\n", node->loop.buffers[0]);
    ef->env->in_loop = false;
    return status;
}

//TODO: need to change the while true by a check_condition
static
void launch_loop(ef_t *ef, ast_t *node)
{
    int status;
    char **save_array = NULL;
    int (*loop_func)(ef_t *, ast_t *, char **) = while_loop;

    signal(SIGINT, exit_child);
    node = get_usr_loop_cmd(node);
    if (node == NULL)
        exit(RETURN_FAILURE);
    save_array = arraydup(node->loop.buffers);
    if (save_array == NULL)
        exit(RETURN_FAILURE);
    if (node->tok.type == T_FOREACH)
        loop_func = foreach_loop;
    while (true)
        status = loop_func(ef, node, save_array);
    free_array(save_array);
    free_array(node->loop.buffers);
    exit(status);
}

int visit_loop(ef_t *ef, ast_t *node)
{
    int status;
    pid_t pid;

    node->loop.buffers = malloc(sizeof(char *) * node->loop.cap);
    if (node == NULL)
        return 84;
    pid = fork();
    if (pid == 0)
        launch_loop(ef, node);
    else
        wait(&status);
    if (WIFEXITED(status))
        ef->history->last_exit_code =
            ef->history->last_exit_code ?: WEXITSTATUS(status);
    return status;
}
