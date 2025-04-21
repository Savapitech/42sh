/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast.h"
#include "u_str.h"

ast_t *parse_and(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL || l_node == NULL)
        return NULL;
    node->tok = ctx->act_tok;
    node->type = N_BIN;
    node->binary.left = l_node;
    node->binary.right = parse_semi(ctx);
    if (node->binary.right == NULL)
        return NULL;
    return node;
}

ast_t *parse_or(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL || l_node == NULL)
        return NULL;
    node->tok = ctx->act_tok;
    node->type = N_BIN;
    node->binary.left = l_node;
    node->binary.right = parse_semi(ctx);
    if (node->binary.right == NULL)
        return NULL;
    return node;
}

static
bool fill_if_node(ast_ctx_t *ctx, ast_t *node, buff_t *buff)
{
    if (!ensure_cond_cap(node))
        return false;
    ctx->str = buff->str;
    node->cond.nodes[node->cond.sz] = parse_semi(ctx);
    if (node->cond.nodes[node->cond.sz] == NULL)
        return false;
    node->cond.sz++;
    return true;
}

static
ast_t *fill_if(ast_ctx_t *ctx, ast_t *node)
{
    buff_t buff = { .str = NULL, 0 };
    char *old_buff = ctx->str;

    while (true) {
        WRITE_CONST(STDOUT_FILENO, IF_PROMPT);
        if (getline(&buff.str, &buff.sz, stdin) < 0)
            return NULL;
        buff.str[strlen(buff.str) - 1] = '\0';
        if (strncmp(buff.str, "endif", 5) == 0)
            break;
        if (!fill_if_node(ctx, node, &buff))
            return NULL;
        buff = (buff_t){ .str = NULL, 0 };
    }
    ctx->str = old_buff;
    return node;
}

ast_t *parse_if(ast_ctx_t *ctx)
{
    ast_t *node = create_node(ctx);

    if (node == NULL)
        return NULL;
    node->tok = ctx->act_tok;
    node->type = N_COND;
    node->cond.cap = DEFAULT_N_COND_CAP;
    node->cond.sz = 0;
    node->cond.nodes =
        (ast_t **)malloc(sizeof *node->cond.nodes * node->cond.cap);
    if ((void *)node->cond.nodes == NULL)
        return NULL;
    node->cond.exp = parse_semi(ctx);
    if (node->cond.exp == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Too few arguments.\n"), NULL;
    return fill_if(ctx, node);
}
