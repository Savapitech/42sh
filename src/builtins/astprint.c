/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "exec.h"
#include "u_str.h"

static
int parse_ast(char *buffer)
{
    ast_ctx_t ctx = { 0, .str = buffer,
        .cap = u_strlen(buffer) + 10 + DEFAULT_AST_CAP,
        .ast = malloc(sizeof *ctx.ast *
                (u_strlen(buffer) + 10 + DEFAULT_AST_CAP)), .parsed_tok = 0 };

    if (ctx.ast == nullptr)
        return RETURN_FAILURE;
    ctx.ast = parse_expression(&ctx);
    print_ast(ctx.ast, &ctx, 0);
    free_ast(&ctx);
    return RETURN_SUCCESS;
}

int builtins_astprint(ef_t *, char **args)
{
    if (args[1] == nullptr)
        return fprintf(stderr, "ast: Too few arguments\n"), RETURN_FAILURE;
    return parse_ast(args[1]);
}
