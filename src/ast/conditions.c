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
ast_t *fill_if(ast_ctx_t *ctx, ast_t *node)
{
    buff_t buff = { .str = NULL, 0 };
    char *old_buff = ctx->str;

    while (true) {
        ensure_list_cap(node);
        WRITE_CONST(STDOUT_FILENO, IF_PROMPT);
        if (getline(&buff.str, &buff.sz, stdin) < 0)
            return NULL;
        buff.str[strlen(buff.str) - 1] = '\0';
        if (strncmp(buff.str, "endif", 5) == 0)
            break;
        ctx->str = buff.str;
        node->list.nodes[node->list.sz] = parse_semi(ctx);
        if (node->list.nodes[node->list.sz] == NULL)
            return NULL;
        buff = (buff_t){ .str = NULL, 0 };
        node->list.sz++;
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
    node->type = N_LST;
    node->list.cap = 2;
    node->list.nodes =
        (ast_t **)malloc(sizeof *node->list.nodes * node->list.cap);
    if ((void *)node->list.nodes == NULL)
        return NULL;
    node->list.nodes[0] = parse_semi(ctx);
    if (node->list.nodes[0] == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Too few arguments.\n"), NULL;
    node->list.sz = 1;
    return fill_if(ctx, node);
}
