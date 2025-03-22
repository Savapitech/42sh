/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "common.h"
#include "exec.h"

int visit_pipe(ef_t *ef)
{
    ast_t *node = ef->ctx->ast;
    int result = RETURN_FAILURE;

    ef->pin_fd = STDIN_FILENO;
    for (size_t i = 0; i < node->list.sz; i++) {
        if (i < node->list.sz - 1)
            pipe(ef->pipes);
        ef->pout_fd = i == node->list.sz - 1 ? STDOUT_FILENO : ef->pipes[1];
        ef->act_node = node->list.nodes[i];
        result = execute(ef);
        if (ef->pin_fd != STDIN_FILENO)
            close(ef->pin_fd);
        if (i < node->list.sz - 1) {
            close(ef->pipes[1]);
            ef->pin_fd = ef->pipes[0];
        }
    }
    return result;
}

int visitor(char *buffer, env_t *env, history_t *history)
{
    ast_ctx_t ctx = { 0, .str = buffer, .cap = DEFAULT_AST_CAP,
        .ast = malloc(sizeof *ctx.ast * DEFAULT_AST_CAP) };
    ef_t ef = { .buffer = buffer, .env = env,
        .history = history, .ctx = &ctx, 0 };
    int result = RETURN_FAILURE;

    history->last_exit_code = 0;
    if (ctx.ast == NULL)
        return RETURN_FAILURE;
    ctx.act_tok = get_next_token(&ctx);
    ctx.ast = parse_expression(&ctx);
    if (ctx.ast == NULL)
        return RETURN_FAILURE;
    if (ctx.ast->type == N_LST && ctx.ast->tok.type == T_PIPE)
        result = visit_pipe(&ef);
    if (ctx.ast->type == N_CMD) {
        ef.act_node = ctx.ast;
        result = execute(&ef);
    }
    return result;
}
