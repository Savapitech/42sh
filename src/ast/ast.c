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
    if (ctx->act_tok.type == T_SEMICOLON)
        return node;
    if (ctx->act_tok.type & (T_ARG | T_REDIRECT | T_APPEND |
        T_IN_REDIRECT | T_HEREDOC)) {
        if (!ensure_node_cap(node))
            return NULL;
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
ast_t *parse_cmd(ast_ctx_t *ctx)
{
    if (ctx->act_tok.type == T_EOF)
        return ctx->ast;
    if (ctx->act_tok.type != T_ARG)
        if (!parser_eat(ctx, T_ARG))
            return NULL;
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
    while (ctx->act_tok.type & (T_ARG | T_PIPE))
        if (!parse_pipe_childs(ctx, node))
            return NULL;
    return node;
}

int shell_loop(env_t *env, int is_a_tty, history_t *history)
{
    char *buffer = NULL;
    size_t buffer_sz;
    size_t buffer_len;

    while (true) {
        if (is_a_tty)
            WRITE_CONST(STDOUT_FILENO, SHELL_PROMPT);
        if (getline(&buffer, &buffer_sz, stdin) == -1)
            break;
        buffer_len = u_strlen(buffer);
        if (buffer_len < 2 || !u_str_is_alnum(buffer)) {
            check_basic_error(buffer);
            continue;
        }
        buffer[buffer_len - 1] = '\0';
        U_DEBUG("Buffer [%lu] [%s]\n", buffer_len, buffer);
        visitor(buffer, env, history);
    }
    return (free(buffer), history->last_exit_code);
}

//TODO: change list.nodes[0] by pars if
ast_t *pars_loop(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);
    char *og_buffer = strdup(ctx->str);

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
    node = get_usr_new_cmd();
}

static
ast_t *parse_semi(ast_ctx_t *ctx)
{
    ast_t *l_node = parse_cmd(ctx);

    if (ctx->act_tok.type & (T_WHILE | T_FOREACH))
        ctx->ast = parse_loop(ctx, l_node);
    if (ctx->act_tok.type == T_PIPE)
        ctx->ast = parse_pipe(ctx, l_node);
    else
        return l_node;
    return ctx->ast;
}

static
ast_t *create_semi_node(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return NULL;
    node->type = N_LST;
    node->list.cap = 2;
    node->list.nodes = (ast_t **)malloc(sizeof(ast_t *) * node->list.cap);
    if ((void *)node->list.nodes == NULL)
        return NULL;
    node->list.sz = 1;
    node->list.nodes[0] = l_node;
    node->tok = ctx->act_tok;
    return node;
}

static
ast_t *fill_semi_node(ast_ctx_t *ctx, ast_t *node)
{
    while (ctx->act_tok.type & (T_SEMICOLON | T_AT)) {
        ctx->act_tok = get_next_token(ctx);
        if (ctx->act_tok.type & (T_SEMICOLON | T_AT))
            continue;
        if (!ensure_list_cap(node))
            return false;
        node->list.nodes[node->list.sz] = parse_semi(ctx);
        if (node->list.nodes[node->list.sz] == NULL)
            return false;
        node->list.sz++;
    }
    return node;
}

static
void skip_semi(ast_ctx_t *ctx)
{
    while (ctx->act_tok.type & (T_SEMICOLON | T_AT))
        ctx->act_tok = get_next_token(ctx);
}

ast_t *parse_expression(ast_ctx_t *ctx)
{
    ast_t *l_node;
    ast_t *node;

    if (ctx->act_tok.type == T_EOF)
        return ctx->ast;
    skip_semi(ctx);
    l_node = parse_semi(ctx);
    if (l_node == NULL)
        return ctx->ast;
    if (ctx->act_tok.type & (T_SEMICOLON | T_AT)) {
        node = create_semi_node(ctx, l_node);
        if (node == NULL)
            return NULL;
        ctx->ast = fill_semi_node(ctx, node);
    }
    ctx->act_tok = get_next_token(ctx);
    return parse_expression(ctx);
}
