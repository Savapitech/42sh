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
bool fill_else_node(ast_ctx_t *ctx, ast_t *node, buff_t *buff)
{
    ctx->str = buff->str;
    node->cond.nodes2[node->cond.sz2] = parse_semi(ctx);
    if (node->cond.nodes2[node->cond.sz2] == NULL)
        return false;
    node->cond.sz2++;
    if (!ensure_cond_cap2(node))
        return false;
    return true;
}

static
bool fill_if_node(ast_ctx_t *ctx, ast_t *node, bool fill_else, buff_t *buff)
{
    if (strncmp(buff->str, "else", 4) == 0)
            return true;
    if (fill_else)
        return fill_else_node(ctx, node, buff);
    ctx->str = buff->str;
    node->cond.nodes[node->cond.sz] = parse_semi(ctx);
    if (node->cond.nodes[node->cond.sz] == NULL)
        return false;
    node->cond.sz++;
    if (!ensure_cond_cap(node))
        return false;
    return true;
}

static
ast_t *fill_if(ast_ctx_t *ctx, ast_t *node)
{
    buff_t buff = { .str = NULL, 0 };
    char *old_buff = ctx->str;
    bool fill_else = false;

    while (true) {
        if (isatty(STDIN_FILENO))
            WRITE_CONST(STDOUT_FILENO, IF_PROMPT);
        if (getline(&buff.str, &buff.sz, stdin) < 0)
            return NULL;
        buff.str[strlen(buff.str) - 1] = '\0';
        if (strncmp(buff.str, "endif", 5) == 0)
            break;
        if (strncmp(buff.str, "else", 4) == 0)
            fill_else = true;
        if (!fill_if_node(ctx, node, fill_else, &buff))
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
    node->cond.cap2 = DEFAULT_N_COND_CAP;
    memset(&node->cond.sz, 0, sizeof node->cond.sz * 2);
    node->cond.nodes =
        (ast_t **)malloc(sizeof *node->cond.nodes * node->cond.cap);
    node->cond.nodes2 =
        (ast_t **)malloc(sizeof *node->cond.nodes * node->cond.cap2);
    if ((void *)node->cond.nodes == NULL || (void *)node->cond.nodes2 == NULL)
        return NULL;
    node->cond.exp = parse_semi(ctx);
    if (node->cond.exp == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Too few arguments.\n"), NULL;
    return fill_if(ctx, node);
}
