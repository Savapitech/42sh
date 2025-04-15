/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** loop
*/

#include <stdlib.h>

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "redirects.h"
#include "u_str.h"

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

//int visit_loop(ef_t *ef, ast_t *node)
//{
//    if (node->tok.type == T_WHILE)
//        
//}
