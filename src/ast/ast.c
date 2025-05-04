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

static
ast_t *parse_arg(ast_ctx_t *ctx, ast_t *node)
{
    ctx->act_tok = get_next_token(ctx);
    if (ctx->act_tok.type & (T_SEMICOLON | T_NEWLINE))
        return node;
    if (ctx->act_tok.type == T_BACKSLASH) {
        ctx->act_tok = get_next_token(ctx);
        if (ctx->act_tok.type == T_EOF)
            return node;
        ctx->act_tok.type = T_ARG;
    }
    if (ctx->act_tok.type & (T_ARG | T_REDIRECT | T_APPEND |
        T_IN_REDIRECT | T_HEREDOC | T_VAR | T_STAR)) {
        if (!ensure_node_cap(node))
            return nullptr;
        node->vector.tokens[node->vector.sz] = ctx->act_tok;
        node->vector.sz++;
        return parse_arg(ctx, node);
    }
    return node;
}

static
ast_t *fill_cmd_node(ast_ctx_t *ctx)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return nullptr;
    node->type = N_CMD;
    node->vector.cap = DEFAULT_N_CMD_CAP;
    node->vector.tokens =
        malloc(sizeof *node->vector.tokens * node->vector.cap);
    if (node->vector.tokens == NULL)
        return nullptr;
    node->tok = ctx->act_tok;
    node->vector.tokens[0] = ctx->act_tok;
    node->vector.sz = 1;
    return parse_arg(ctx, node);
}

/*
 * Removed this check to do && || if, nothing has changed,
 * to be seen in case of trouble,
 * putting it back this may solve the problem but will break the && || if
 *   if (ctx->act_tok.type == T_EOF)
 *       return ctx->ast;
 */
ast_t *parse_cmd(ast_ctx_t *ctx)
{
    if (ctx->act_tok.type != T_ARG) {
        if (ctx->act_tok.type & (T_WHILE))
            return nullptr;
        if (!parser_eat(ctx, T_ARG))
            return nullptr;
    }
    return fill_cmd_node(ctx);
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

ast_t *parse_pipe(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return nullptr;
    node->type = N_LST;
    node->tok = ctx->act_tok;
    node->list.cap = DEFAULT_N_LST_CAP;
    node->list.nodes = (ast_t **)malloc(sizeof(ast_t *) * node->list.cap);
    if ((void *)node->list.nodes == NULL)
        return nullptr;
    node->list.sz = 1;
    node->list.nodes[0] = l_node;
    if (!parser_eat(ctx, T_ARG))
        return nullptr;
    while (ctx->act_tok.type & (T_ARG | T_PIPE))
        if (!parse_pipe_childs(ctx, node))
            return nullptr;
    return node;
}

ast_t *parse_condition(ast_ctx_t *ctx)
{
    ast_t *l_node = parse_cmd(ctx);

    if (l_node == NULL)
        return nullptr;
    switch (ctx->act_tok.type) {
        case T_PIPE:
            ctx->ast = parse_pipe(ctx, l_node);
            break;
        default:
            return l_node;
    }
    return ctx->ast;
}

ast_t *parse_semi(ast_ctx_t *ctx)
{
    ast_t *l_node = parse_condition_and(ctx);

    if (l_node == NULL)
        return nullptr;
    if (ctx->act_tok.type == T_OR) {
        ctx->ast = parse_or(ctx, l_node);
        return ctx->ast;
    }
    return l_node;
}

static
ast_t *create_semi_node(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return nullptr;
    node->type = N_LST;
    node->list.cap = DEFAULT_N_LST_CAP;
    node->list.nodes = (ast_t **)malloc(sizeof(ast_t *) * node->list.cap);
    if ((void *)node->list.nodes == NULL)
        return nullptr;
    node->list.sz = 1;
    node->list.nodes[0] = l_node;
    node->tok = ctx->act_tok;
    return node;
}

static
ast_t *fill_semi_node(ast_ctx_t *ctx, ast_t *node)
{
    while (ctx->act_tok.type & (T_SEMICOLON | T_NEWLINE)) {
        ctx->act_tok = get_next_token(ctx);
        if (ctx->act_tok.type & (T_SEMICOLON | T_NEWLINE))
            continue;
        if (!ensure_list_cap(node))
            return nullptr;
        node->list.nodes[node->list.sz] = parse_semi(ctx);
        if (node->list.nodes[node->list.sz] == NULL)
            return nullptr;
        node->list.sz++;
    }
    return node;
}

ast_t *parse_expression(ast_ctx_t *ctx)
{
    ast_t *l_node;
    ast_t *node;

    if (ctx->act_tok.type == T_EOF)
        return ctx->ast;
    ctx->act_tok = get_next_token(ctx);
    skip_semi(ctx);
    l_node = parse_semi(ctx);
    if (l_node == NULL)
        return ctx->ast;
    if (ctx->act_tok.type & (T_SEMICOLON | T_NEWLINE)) {
        node = create_semi_node(ctx, l_node);
        if (node == NULL)
            return nullptr;
        ctx->ast = fill_semi_node(ctx, node);
    }
    return parse_expression(ctx);
}
