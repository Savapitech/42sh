/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <ctype.h>

#include "ast.h"
#include "debug.h"
#include "u_str.h"

const tokens_list_t TOKENS_LIST[] = {
    { T_SEMICOLON, ";", 1, "T_SEMICOLON" },
    { T_LEFT_QUOTE, "\"", 1, "T_LEFT_QUOTE" },
    { T_RIGHT_QUOTE, "\"", 1, "T_RIGHT_QUOTE" },
    { T_AND, "&&", 2, "T_AND" },
    { T_OR, "||", 2, "T_OR" },
    { T_PIPE, "|", 1, "T_PIPE" },
    { T_BACKTICK, "`", 1, "T_BACKTICK" },
    { T_LEFT_PARENT, "(", 1, "T_LEFT_PARENT" },
    { T_RIGHT_PARENT, ")", 1, "T_RIGHT_PARENT" },
    { T_PREV_CMD, "!!", 2, "T_PREV_CMD" },
    { T_VAR, "$", 1, "T_VAR" },
    { T_APPEND, ">>", 2, "T_APPEND" },
    { T_REDIRECT, ">", 1, "T_REDIRECT" },
    { T_AT, "@", 1, "T_AT" },
    { T_HEREDOC, "<<", 2, "T_HEREDOC" },
    { T_IN_REDIRECT, "<", 1, "T_IN_REDIRECT" },
    { T_EOF, "\0", 1, "T_EOF" }
};

const size_t TOKENS_LIST_SZ = sizeof TOKENS_LIST / sizeof *TOKENS_LIST;

static
void get_arg_token(ast_ctx_t *ctx, int *found_token)
{
    for (size_t i = 0; i < TOKENS_LIST_SZ; i++) {
        if (u_strncmp(ctx->str, TOKENS_LIST[i].str,
            TOKENS_LIST[i].sz) == 0) {
            *found_token = 1;
            break;
        }
    }
    if (!*found_token)
        ctx->str++;
}

token_t get_next_token(ast_ctx_t *ctx)
{
    char *start;
    int found_token = 0;

    while (*ctx->str != '\0' && isblank(*ctx->str))
        ctx->str++;
    for (size_t i = 0; i < TOKENS_LIST_SZ; i++) {
        if (u_strncmp(ctx->str, TOKENS_LIST[i].str, TOKENS_LIST[i].sz) == 0) {
            U_DEBUG("Token %-14s [%.*s]\n", TOKENS_LIST[i].name,
                (int)TOKENS_LIST[i].sz, ctx->str);
            ctx->str += TOKENS_LIST[i].sz;
            return (token_t){ TOKENS_LIST[i].type,
                ctx->str - TOKENS_LIST[i].sz, TOKENS_LIST[i].sz };
        }
    }
    start = ctx->str;
    while (*ctx->str && !isblank(*ctx->str) && !found_token)
        get_arg_token(ctx, &found_token);
    U_DEBUG("Token T_ARG          [%.*s]\n", (int)(ctx->str - start), start);
    return (token_t){ .type = T_ARG, .str = start,
        .sz = (size_t)(ctx->str - start) };
}
