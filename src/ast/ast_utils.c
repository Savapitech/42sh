/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "u_mem.h"

void print_ast(ast_t *ast, ast_ctx_t *ctx, size_t depth)
{
    for (size_t i = 0; i < depth; i++)
        fprintf(stderr, " ");
    if (depth)
        fprintf(stderr, "- ");
    fprintf(stderr, "(%.*s)\n", (int)ast->tok.sz, ast->tok.str);
    if (ast->type == N_BIN) {
        print_ast(ast->binary.left, ctx, depth + 1);
        print_ast(ast->binary.right, ctx, depth + 1);
    }
    if (ast->type == N_CMD) {
        for (size_t i = 1; i < ast->vector.sz; i++)
            fprintf(stderr, "%*s - (%.*s)\n", (int)depth, "",
                (int)ast->vector.tokens[i].sz, ast->vector.tokens[i].str);
    }
    if (ast->type == N_LST) {
        for (size_t i = 0; i < ast->list.sz; i++)
            print_ast(ast->list.nodes[i], ctx, depth + 1);
    }
}

ast_t *create_node(ast_ctx_t *ctx)
{
    ast_t *new_ast;

    if (ctx->ast == NULL)
        return nullptr;
    if (ctx->sz + 1 == ctx->cap) {
        new_ast = u_realloc(ctx->ast, sizeof *ctx->ast * ctx->sz,
            sizeof *ctx->ast * (ctx->cap << 1));
        if (new_ast == NULL)
            return nullptr;
        ctx->ast = new_ast;
        ctx->cap <<= 1;
    }
    ctx->ast[ctx->sz] = (ast_t){ 0 };
    ctx->sz++;
    return ctx->ast + ctx->sz - 1;
}

static
void free_ast_nodes(ast_t *ast)
{
    if (ast == nullptr)
        return;
    if (ast->type == N_BIN) {
        free_ast_nodes(ast->binary.left);
        free_ast_nodes(ast->binary.right);
    }
    if (ast->type == N_CMD)
        free(ast->vector.tokens);
    if (ast->type == N_LST) {
        for (size_t i = 0; i < ast->list.sz; i++)
            free_ast_nodes(ast->list.nodes[i]);
        free(ast->list.nodes);
    }
}

void free_ast(ast_ctx_t *ctx)
{
    free_ast_nodes(ctx->ast);
    free(ctx->first_node);
}
