/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "u_mem.h"
#include "u_str.h"

bool parser_eat(ast_ctx_t *ctx, token_type_t expected)
{
    token_type_t prev_tok_type = ctx->act_tok.type;

    ctx->act_tok = get_next_token(ctx);
    if (!(ctx->act_tok.type & expected)) {
        if (prev_tok_type == T_PIPE)
            WRITE_CONST(STDERR_FILENO, "Invalid null command.\n");
        else {
            WRITE_CONST(STDERR_FILENO, "Parse error near \"");
            write(STDERR_FILENO, ctx->act_tok.str, ctx->act_tok.sz);
            WRITE_CONST(STDERR_FILENO, "\"\n");
        }
        return false;
    }
    return true;
}

ast_t *create_node(ast_ctx_t *ctx)
{
    ast_t *new_ast;

    if (ctx->ast == NULL)
        return NULL;
    if (ctx->sz + 1 == ctx->cap) {
        new_ast = u_realloc(ctx->ast, sizeof *ctx->ast * ctx->sz,
            sizeof *ctx->ast * (ctx->cap << 1));
        if (new_ast == NULL)
            return NULL;
        ctx->ast = new_ast;
        ctx->cap <<= 1;
    }
    ctx->ast[ctx->sz] = (ast_t){ 0 };
    ctx->sz++;
    return ctx->ast + ctx->sz - 1;
}

bool ensure_node_cap(ast_t *node)
{
    token_t *temp;

    if (node->vector.sz + 1 < node->vector.cap)
        return true;
    temp = u_realloc(node->vector.tokens,
        sizeof *node->vector.tokens * node->vector.sz,
        sizeof *node->vector.tokens * node->vector.cap << 1);
    if (temp == NULL)
        return false;
    node->vector.cap <<= 1;
    node->vector.tokens = temp;
    return true;
}

bool ensure_list_cap(ast_t *node)
{
    ast_t **temp;

    if (node->list.sz + 1 < node->vector.cap)
        return true;
    temp = (ast_t **)u_realloc((void *)node->list.nodes,
        sizeof *node->list.nodes * node->list.sz,
        sizeof *node->list.nodes * node->vector.cap << 1);
    if ((void *)temp == NULL)
        return false;
    node->list.cap <<= 1;
    node->list.nodes = temp;
    return true;
}

void free_ast(ast_ctx_t *ctx)
{
    for (size_t i = 0; i < ctx->sz; i++) {
        if (ctx->first_node[i].type == N_LST)
            free((void *)ctx->first_node[i].list.nodes);
        if (ctx->first_node[i].type == N_CMD)
            free(ctx->first_node[i].vector.tokens);
    }
    free(ctx->first_node);
}
