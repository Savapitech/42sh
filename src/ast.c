/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast.h"
#include "u_mem.h"
#include "u_str.h"

static
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

static
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

static
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

static
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

static
ast_t *parse_arg(ast_ctx_t *ctx, ast_t *node)
{
    ctx->act_tok = get_next_token(ctx);
    if (ctx->act_tok.type == T_ARG) {
        if (!ensure_node_cap(node))
            return NULL;
        node->vector.tokens[node->vector.sz] = ctx->act_tok;
        node->vector.sz++;
        return parse_arg(ctx, node);
    }
    return node;
}

static
ast_t *parse_cmd(ast_ctx_t *ctx)
{
    ast_t *node;

    if (ctx->act_tok.type == T_EOF)
        return ctx->ast;
    if (ctx->act_tok.type != T_ARG)
        if (!parser_eat(ctx, T_ARG))
            return NULL;
    node = create_node(ctx);
    if (node == NULL)
        return NULL;
    node->type = N_CMD;
    node->vector.cap = 2;
    node->vector.tokens =
        malloc(sizeof *node->vector.tokens * node->vector.cap);
    if (node->vector.tokens == NULL)
        return NULL;
    node->tok = ctx->act_tok;
    node->vector.sz = 0;
    return parse_arg(ctx, node);
}

static
bool parse_pipe_childs(ast_ctx_t *ctx, ast_t *node)
{
    if (!ensure_list_cap(node))
        return false;
    node->list.nodes[node->list.sz] = parse_cmd(ctx);
    if (node->list.nodes[node->list.sz] == NULL)
        return false;
    node->list.sz++;
    return true;
}

static
ast_t *parse_pipe(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return NULL;
    node->type = N_LST;
    node->tok = ctx->act_tok;
    node->list.cap = 2;
    node->list.nodes = (ast_t **)malloc(sizeof(ast_t *) * node->list.cap);
    if ((void *)node->list.nodes == NULL)
        return NULL;
    node->list.sz = 1;
    node->list.nodes[0] = l_node;
    if (!parser_eat(ctx, T_ARG))
        return NULL;
    while (ctx->act_tok.type == T_ARG || ctx->act_tok.type == T_PIPE)
        if (!parse_pipe_childs(ctx, node))
            return NULL;
    return node;
}

ast_t *parse_expression(ast_ctx_t *ctx)
{
    ast_t *l_node = parse_cmd(ctx);

    if (l_node == NULL || ctx->act_tok.type == T_EOF)
        return ctx->ast;
    if (ctx->act_tok.type == T_SEMICOLON) {
        if (parse_cmd(ctx) == NULL)
            return NULL;
        if (ctx->act_tok.type == T_EOF)
            return ctx->ast;
    }
    if (ctx->act_tok.type == T_PIPE)
        ctx->ast = parse_pipe(ctx, l_node);
    ctx->act_tok = get_next_token(ctx);
    return parse_expression(ctx);
}
