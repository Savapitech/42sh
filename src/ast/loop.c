/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** loop
*/
#include "ast.h"
#include <stdlib.h>

ast_t *parse_loop(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return NULL;
    node->type = N_LOP;
    node->tok = ctx->act_tok;
    node->loop.cap = 2;
    node->loop.buffers = (char **)malloc(sizeof(char *) * node->loop.cap);
    if ((void *)node->loop.buffers == NULL)
        return NULL;
    node->loop.sz = 1;
    node->loop.condition = l_node;
    return node;
}
