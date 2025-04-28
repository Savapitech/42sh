/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <ctype.h>
#include <unistd.h>

#include "ast.h"
#include "debug.h"
#include "u_str.h"

const tokens_list_t TOKENS_LIST[] = {
    { T_SEMICOLON, ";", 1, "T_SEMICOLON" },
    { T_QUOTES, "\'", 1, "T_QUOTES"},
    { T_DQUOTES, "\"", 1, "T_DQUOTES"},
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
    { T_HEREDOC, "<<", 2, "T_HEREDOC" },
    { T_IN_REDIRECT, "<", 1, "T_IN_REDIRECT" },
    { T_WHILE, "while", 5, "T_WHILE"},
    { T_IF, "if", 2, "T_IF"},
    { T_THEN, "then", 4, "T_THEN"},
    { T_ELSE, "else", 4, "T_ELSE"},
    { T_ENDIF, "endif", 5, "T_ENDIF"},
    { T_EOF, "\0", 1, "T_EOF" }
};

const size_t TOKENS_LIST_SZ = sizeof TOKENS_LIST / sizeof *TOKENS_LIST;

const tokens_list_t CLOSABLE[] = {
    { T_RIGHT_PARENT, ")", 1, "T_RIGHT_PARENT" },
    { T_LEFT_PARENT, "(", 1, "T_LEFT_PARENT" },
    { T_BACKTICK, "`", 1, "T_BACKTICK" },
    { T_QUOTES, "\'", 1, "T_QUOTES"},
    { T_DQUOTES, "\"", 1, "T_DQUOTES"}
};

const size_t CLOSABLE_LIST_SZ = sizeof CLOSABLE / sizeof *CLOSABLE;

void skip_semi(ast_ctx_t *ctx)
{
    while (ctx->act_tok.type == T_SEMICOLON)
        ctx->act_tok = get_next_token(ctx);
}

bool parser_eat(ast_ctx_t *ctx, token_type_t expected)
{
    token_type_t prev_tok_type = ctx->act_tok.type;

    ctx->act_tok = get_next_token(ctx);
    if (!(ctx->act_tok.type & expected)) {
        if (prev_tok_type == T_PIPE)
            WRITE_CONST(STDERR_FILENO, "Invalid null command.\n");
        else
            dprintf(STDERR_FILENO, "Parse error near \"%.*s\"\n",
                (int)ctx->ast->tok.sz, ctx->ast->tok.str);
        return false;
    }
    return true;
}

bool check_closable(token_t actual_token)
{
    if (actual_token.type == 0)
        return true;
    for (size_t i = 0; i < CLOSABLE_LIST_SZ; i++) {
        if (actual_token.type == CLOSABLE[i].type)
            return true;
    }
    return false;
}

static
void get_arg_token(ast_ctx_t *ctx, int *found_token, token_t acutal_tok)
{
    if (check_closable(acutal_tok)){
        ctx->str++;
        if (u_strncmp(ctx->str, acutal_tok.str, acutal_tok.sz) == 0){
            *found_token = 1;
            ctx->str++;
            return;
        }
    }
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

static
token_t handle_token_type(ast_ctx_t *ctx)
{
    for (size_t i = 0; i < TOKENS_LIST_SZ; i++) {
        if (u_strncmp(ctx->str, TOKENS_LIST[i].str, TOKENS_LIST[i].sz) == 0) {
            U_DEBUG("Token %-14s [%.*s]\n", TOKENS_LIST[i].name,
                (int)TOKENS_LIST[i].sz, ctx->str);
            ctx->str += TOKENS_LIST[i].sz;
            return (token_t){ TOKENS_LIST[i].type,
                ctx->str - TOKENS_LIST[i].sz, TOKENS_LIST[i].sz };
        }
    }
    return (token_t){0, NULL, 0};
}

void format_for_closable(ast_ctx_t *ctx, token_t *actual_token)
{
    if (actual_token->type != 0)
        ctx->str -= actual_token->sz;
    if (actual_token->type == T_LEFT_PARENT)
        *actual_token =
        (token_t){ T_RIGHT_PARENT, ")", 1};
}

token_t get_next_token(ast_ctx_t *ctx)
{
    char *start;
    int found_token = 0;
    token_t actual_token;

    while (*ctx->str != '\0' && isblank(*ctx->str))
        ctx->str++;
    actual_token = handle_token_type(ctx);
    if (!check_closable(actual_token))
        return actual_token;
    format_for_closable(ctx, &actual_token);
    start = ctx->str;
    while ((*ctx->str && !found_token && (!isblank(*ctx->str) ||
        check_closable(actual_token))))
        get_arg_token(ctx, &found_token, actual_token);
    U_DEBUG("Token T_ARG          [%.*s]\n", (int)(ctx->str - start), start);
    return (token_t){ .type = T_ARG, .str = start,
        .sz = (size_t)(ctx->str - start) };
}
