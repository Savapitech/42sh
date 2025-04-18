/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include "ast.h"

ast_t *parse_and(ast_ctx_t *ctx, ast_t *l_node)
{
    ast_t *node = create_node(ctx);

    if (node == NULL || l_node == NULL)
        return NULL;
    node->tok = ctx->act_tok;
    node->type = N_BIN;
    node->binary.left = l_node;
    node->binary.right = parse_condition(ctx);
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
    node->binary.right = parse_condition(ctx);
    if (node->binary.right == NULL)
        return NULL;
    return node;
}
