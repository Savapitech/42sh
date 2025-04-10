/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "redirects.h"
#include "u_str.h"

#include "debug.h"

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
    ef->skip_i = 0;
    ef->skip_sz = 0;
    ef->rout_fd = 0;
    ef->rin_fd = 0;
    for (size_t i = 0; i < ef->act_node->vector.sz; i++) {
        if (!handle_in_redirect(ef, ef->act_node, i, ef->act_node->vector.sz)
        || !handle_out_redirect(ef, ef->act_node, i, ef->act_node->vector.sz)
        || !handle_heredoc(ef, ef->act_node, i, ef->act_node->vector.sz))
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
int visit_pipe(ef_t *ef, size_t i, ast_t *node)
{
    int result = RETURN_FAILURE;

    if (i < node->list.sz - 1)
        if (pipe(ef->pipes) < 0)
            return (puterror("pipe"), -1);
    ef->pout_fd = i == node->list.sz - 1 ? STDOUT_FILENO : ef->pipes[1];
    ef->act_node = node->list.nodes[i];
    if (!ef->act_node)
        return -1;
    result = visit_cmd(ef);
    if (result == -1)
        return RETURN_FAILURE;
    if (ef->pin_fd != STDIN_FILENO)
        close(ef->pin_fd);
    if (i < node->list.sz - 1) {
        close(ef->pipes[1]);
        ef->pin_fd = ef->pipes[0];
    }
    return result;
}

static
int visit_pipes(ef_t *ef)
{
    ast_t *node = ef->act_node;
    int result = RETURN_FAILURE;

    ef->p_i = 0;
    ef->p_sz = node->list.sz;
    ef->pin_fd = STDIN_FILENO;
    ef->flags |= F_PIPE;
    for (size_t i = 0; i < node->list.sz; i++) {
        ef->p_i = i;
        result = visit_pipe(ef, i, node);
        if (result == -1)
            break;
    }
    return result;
}

static
int visit_list(ef_t *ef, ast_t *node)
{
    int result = RETURN_FAILURE;

    if (node->tok.type == T_PIPE) {
        ef->act_node = node;
        return visit_pipes(ef);
    }
    for (size_t i = 0; i < node->list.sz; i++) {
        ef->flags &= ~F_PIPE;
        ef->act_node = node->list.nodes[i];
        if (node->list.nodes[i]->type == N_LST &&
            node->list.nodes[i]->tok.type == T_PIPE)
            result = visit_pipes(ef);
        ef->pin_fd = STDIN_FILENO;
        ef->pout_fd = STDOUT_FILENO;
        if (node->list.nodes[i]->type == N_CMD)
            result = visit_cmd(ef);
        if (node->tok.type == T_AT)
            sleep(3);
    }
    return result;
}

static
int visitor_launcher(ef_t *ef)
{
    int result = RETURN_FAILURE;

    ef->ctx->act_tok = get_next_token(ef->ctx);
    ef->ctx->ast = parse_expression(ef->ctx);
    if (ef->ctx->ast == NULL)
        return RETURN_FAILURE;
    if (ef->ctx->ast->type == N_LST)
        result = visit_list(ef, ef->ctx->ast);
    if (ef->ctx->ast->type == N_CMD) {
        ef->act_node = ef->ctx->ast;
        result = visit_cmd(ef);
    }
    return result;
}

static
void remove_trailing_semi(char *str)
{
    for (size_t len = u_strlen(str); len > 0; len--) {
        if (str[len] != '\0' && str[len] != '\n' && str[len] != ';')
            break;
        if (str[len] == ';')
            str[len] = '\0';
    }
}

int visitor(char *buffer, builtin_handler_t *builtin_handler)
{
    ast_ctx_t ctx = { 0, .str = buffer, .cap = u_strlen(buffer) + 10,
        .ast = malloc(sizeof *ctx.ast * (u_strlen(buffer) + 10)) };
    ef_t ef = { .buffer = buffer, .env = builtin_handler->env,
        .history = builtin_handler->history, .ctx
        = &ctx, .pout_fd = STDOUT_FILENO, .flags = 0, 0 };
    int result = RETURN_FAILURE;

    ctx.first_node = ctx.ast;
    remove_trailing_semi(ctx.str);
    builtin_handler->history->last_exit_code = 0;
    if (ctx.ast == NULL)
        return RETURN_FAILURE;
    result = visitor_launcher(&ef);
    if (ef.flags & F_EXIT)
        builtins_exit(&ef, NULL);
    free_ast(&ctx);
    return result == -1 ? RETURN_FAILURE : result;
}
