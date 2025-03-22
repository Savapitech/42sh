/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "common.h"
#include "exec.h"
#include "u_str.h"

static
bool handle_out_redirect(ef_t *ef, ast_t *node, size_t i, size_t sz)
{
    if (!(node->vector.tokens[i].type & (T_REDIRECT | T_APPEND)))
        return true;
    if (i >= sz || node->vector.tokens[i + 1].type != T_ARG)
        return (WRITE_CONST(STDERR_FILENO,
            "Missing name for redirect.\n"), false);
    ef->skip_i = i;
    ef->skip_sz = 2;
    node->vector.tokens[i + 1].str[node->vector.tokens[i + 1].sz] = '\0';
    ef->rout_fd = open(node->vector.tokens[i + 1].str, O_CREAT | O_WRONLY |
        (node->vector.tokens[i].type == T_APPEND ? O_APPEND : O_TRUNC), 0644);
    if (ef->rout_fd < 0)
        return (puterror(node->vector.tokens[i + 1].str), false);
    ef->out_fd = ef->rout_fd;
    return true;
}

static
bool handle_in_redirect(ef_t *ef, ast_t *node, size_t i, size_t sz)
{
    if (node->vector.tokens[i].type != T_IN_REDIRECT)
        return true;
    if (i >= sz || node->vector.tokens[i + 1].type != T_ARG)
        return (WRITE_CONST(STDERR_FILENO,
            "Missing name for redirect.\n"), false);
    ef->skip_i = i;
    ef->skip_sz = 2;
    node->vector.tokens[i + 1].str[node->vector.tokens[i + 1].sz] = '\0';
    ef->rin_fd = open(node->vector.tokens[i + 1].str, O_RDONLY);
    if (ef->rin_fd < 0)
        return (puterror(node->vector.tokens[i + 1].str), false);
    ef->in_fd = ef->rin_fd;
    return true;
}

/*
 * ef->in_fd = ef->pin_fd;
 * ef->out_fd = ef->out_fd;
 * set used fds, to not close the wrong fd in visit pipe or handle redirect
*/
static
int visit_cmd(ef_t *ef)
{
    int result;

    ef->in_fd = ef->pin_fd;
    ef->out_fd = ef->pout_fd;
    for (size_t i = 0; i < ef->act_node->vector.sz; i++) {
        if (!handle_in_redirect(ef, ef->act_node, i, ef->act_node->vector.sz))
            return -1;
        if (!handle_out_redirect(ef, ef->act_node, i, ef->act_node->vector.sz))
            return -1;
    }
    result = execute(ef);
    if (ef->rout_fd)
        close(ef->rout_fd);
    if (ef->rin_fd)
        close(ef->rin_fd);
    return result;
}

static
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
        result = visit_cmd(ef);
        if (result == -1)
            return RETURN_FAILURE;
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
        .history = history, .ctx = &ctx, .pout_fd = STDOUT_FILENO, 0 };
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
        result = visit_cmd(&ef);
    }
    return result == -1 ? RETURN_FAILURE : result;
}
