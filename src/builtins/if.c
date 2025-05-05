/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "common.h"
#include "debug.h"
#include "exec.h"
#include "repl.h"
#include "u_str.h"

static
int get_argc(char **args)
{
    int i = 0;

    for (; args[i] != nullptr; i++);
    return i;
}

static
bool ensure_cmds_cap(char ***cmd, size_t sz, size_t *cap)
{
    char **tmp;

    if (sz + 1 < *cap)
        return true;
    tmp = (char **)realloc(*cmd, sizeof(char *) * (*cap << 1));
    if (!tmp)
        return false;
    *cap <<= 1;
    *cmd = tmp;
    return true;
}

static
bool init_block(cmd_block_t *blk)
{
    blk->size = 0;
    blk->cap = DEFAULT_N_CMD_CAP;
    blk->cmds = (char **)malloc(sizeof(char *) * blk->cap);
    return blk->cmds != NULL;
}

static
void free_block(cmd_block_t *blk)
{
    for (size_t i = 0; i < blk->size; i++)
        free(blk->cmds[i]);
    free((void *)blk->cmds);
}

static
bool append_block(cmd_block_t *blk, const char *line)
{
    if (!ensure_cmds_cap(&blk->cmds, blk->size, &blk->cap))
        return false;
    blk->cmds[blk->size] = strdup(line);
    blk->size++;
    return true;
}

static
bool handle_if_buff(if_ctx_t *ctx, cmd_block_t *then_blk,
    cmd_block_t *else_blk, bool *in_else)
{
    if (strcmp(ctx->buff->str, "else") == 0) {
        *in_else = true;
        return true;
    }
    if (strcmp(ctx->buff->str, "endif") == 0)
        return false;
    if (!*in_else) {
        if (!append_block(then_blk, ctx->buff->str))
            return false;
    } else
        if (!append_block(else_blk, ctx->buff->str))
            return false;
    return true;
}

static
bool read_if_blocks(ef_t *ef, if_ctx_t *ctx, cmd_block_t *then_blk,
    cmd_block_t *else_blk)
{
    bool in_else = false;

    while (true) {
        if (isatty(ef->exec_ctx->read_fd))
            WRITE_CONST(ctx->ef->out_fd, IF_PROMPT);
        if (getline(&ctx->buff->str, &ctx->buff->sz, stdin) == -1)
            return false;
        ctx->buff->str[strcspn(ctx->buff->str, "\n")] = '\0';
        if (strcmp(ctx->buff->str, "else") == 0) {
            in_else = true;
            continue;
        }
        if (strcmp(ctx->buff->str, "endif") == 0)
            break;
        if (!handle_if_buff(ctx, then_blk, else_blk, &in_else))
            return false;
    }
    return true;
}

static
void exec_block(cmd_block_t *blk, ef_t *ef)
{
    for (size_t i = 0; i < blk->size; i++)
        visitor(blk->cmds[i], ef->exec_ctx);
}

static
bool handle_if_logic(ef_t *ef, bool cond, char *last)
{
    if_ctx_t ctx = {.buff = &(buff_t){ .str = nullptr, .sz = 0 }, .ef = ef};
    cmd_block_t then_blk;
    cmd_block_t else_blk;

    if (strcmp("then", last) != 0)
        return cond ? visitor(last, ef->exec_ctx), true : true;
    if (!init_block(&then_blk) || !init_block(&else_blk))
        return false;
    if (!read_if_blocks(ef, &ctx, &then_blk, &else_blk))
        return false;
    if (cond)
        exec_block(&then_blk, ef);
    else
        exec_block(&else_blk, ef);
    free_block(&then_blk);
    free_block(&else_blk);
    free(ctx.buff->str);
    return true;
}

int builtins_if(ef_t *ef, char **args)
{
    int result;
    char *last;

    if (args[1] == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Too few arguments.\n"),
            RETURN_FAILURE;
    if (args[2] == NULL)
        return WRITE_CONST(STDERR_FILENO, "if: Empty if.\n"), RETURN_FAILURE;
    last = strdup(args[get_argc(args) - 1]);
    if (!last)
        return RETURN_FAILURE;
    args[get_argc(args) - 1] = nullptr;
    result = builtins_expr(ef, args);
    U_DEBUG("If expr result [%d]\n", result);
    if (result == -1)
        return free(last), RETURN_FAILURE;
    if (!handle_if_logic(ef, result != 0, last))
        return free(last), RETURN_FAILURE;
    return free(last), RETURN_SUCCESS;
}
