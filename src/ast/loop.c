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

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "redirects.h"
#include "u_str.h"
#include "u_mem.h"

ast_t *parse_loop(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return NULL;
    node->type = N_LOP;
    node->tok = ctx->act_tok;
    node->loop.cap = 2;
    node->loop.buffers = NULL;
    node->loop.sz = 0;
    node->loop.condition = l_node;
    return node;
}

static
ast_t *buffers_realloc(ast_t *node)
{
    char **new_buffers = u_realloc(node->loop.buffers, sizeof
        *node->loop.buffers * node->loop.sz, sizeof
        *node->loop.buffers * (node->loop.cap << 1));

    if (new_buffers == NULL)
        return NULL;
    node->loop.buffers = new_buffers;
    node->loop.cap <<= 1;
    return node;
}

static
ast_t *get_usr_loop_cmd(ast_t *node)
{
    char prompt[9] = "foreach?";
    size_t buffer_len;

    node->loop.buffers = malloc(sizeof(char *) * node->loop.cap);
    node->loop.buffers[node->loop.sz] = NULL;
    if (node->tok.type == T_WHILE)
        strcpy(prompt, "while?");
    while (!strcmp("end", node->loop.buffers[node->loop.sz])){
        printf("%s", prompt);
        if (node->loop.sz >= node->loop.cap)
            node = buffers_realloc(node);
        if (node == NULL)
            return NULL;
        getline(&(node->loop.buffers[node->loop.sz - 1]), &buffer_len, stdin);
        buffer_len = u_strlen(node->loop.buffers[node->loop.sz - 1]);
        node->loop.buffers[node->loop.sz - 1][buffer_len - 1] = '\0';
        node->loop.sz++;
    }
    node->loop.buffers[node->loop.sz] = NULL;
    return node;
}

//TODO: need to change the while true by a check_condition
int visit_loop(ef_t *ef, ast_t *node)
{
    node = get_usr_loop_cmd(node);
    if (node == NULL)
        return 84;
    while (true){
        for (size_t i = 0; i < node->loop.sz; i++)
            visitor(node->loop.buffers[i], ef->env, ef->history);
    }
}
