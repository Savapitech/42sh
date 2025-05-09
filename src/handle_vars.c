/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** handle_vars
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast.h"
#include "env.h"
#include "exec.h"
#include "local.h"

char *get_values(exec_ctx_t *ctx, char *key)
{
    char *r_char = nullptr;

    r_char = get_env_value(ctx->env, key);
    if (r_char == NULL)
        r_char = get_local_value(ctx->local, key);
    if (r_char == NULL) {
        fprintf(stderr, "%s: Undefined variable.\n", key);
        return nullptr;
    }
    return r_char;
}

static
char *take_next_parenthese_arg(ast_t *node, size_t *in_str, const size_t *i)
{
    size_t end = 0;
    char *buff;

    while (node->vector.tokens[*i].sz > *in_str &&
        isblank(node->vector.tokens[*i].str[*in_str]))
        *in_str = *in_str + 1;
    for (; node->vector.tokens[*i].sz > *in_str + end &&
        !isblank(node->vector.tokens[*i].str[*in_str + end]); end++);
    buff = strndup(&node->vector.tokens[*i].str[*in_str], end);
    *in_str += end;
    return buff;
}

static
bool handle_parentheses(ast_t *node, size_t *i, args_t *args)
{
    size_t in_str = 0;
    char *vl;

    while (in_str < node->vector.tokens[*i].sz){
        if (!ensure_args_capacity(args))
            return false;
        vl = take_next_parenthese_arg(node, &in_str, i);
        if (vl == NULL)
            return free((void *)args->args), false;
        args->args[args->sz] = vl;
        args->sz++;
    }
    args->args[args->sz] = nullptr;
    args->sz--;
    return true;
}

static
bool check_parentheses(ast_t *node, size_t *i, exec_ctx_t *ctx, args_t *args)
{
    if (!strchr("()", node->vector.tokens[*i].str[0]))
        return true;
    if (strlen(node->vector.tokens[*i].str) == 1 ||
        (node->vector.tokens[*i].str[0] == '(' &&
        node->vector.tokens[*i].str[node->vector.tokens[*i].sz - 1] != ')'))
        return (fprintf(stderr, "Too many ('s.\n"), true);
    if (node->vector.tokens[*i].str[0] == ')')
        return (fprintf(stderr, "Too many )'s.\n"), true);
    node->vector.tokens[*i].str[node->vector.tokens[*i].sz - 1] = '\0';
    node->vector.tokens[*i].str = &node->vector.tokens[*i].str[1];
    node->vector.tokens[*i].sz -= 2;
    return handle_parentheses(node, i, args);
}

static
bool format_quotes(ast_t *node, char be_matched, const size_t *i)
{
    char *last_quote = strrchr(node->vector.tokens[*i].str, be_matched);

    if (last_quote == NULL)
        return (fprintf(stderr, "Unmatched \'%c\'.\n", be_matched), false);
    node->vector.tokens[*i].sz -= 2;
    if (isblank(last_quote[1]) || be_matched == '`'){
        last_quote[0] = '\0';
        return true;
    }
    memmove(&last_quote[0], &last_quote[1], strlen(last_quote));
    last_quote = strchr(node->vector.tokens[*i].str, be_matched);
    if (strchr(node->vector.tokens[*i].str, be_matched))
        return (fprintf(stderr, "Unmatched \'%c\'.\n", be_matched), false);
    node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
    return true;
}

static
char *init_move(ast_t *node, size_t *i, size_t *sz_before)
{
    char *move = NULL;

    for (; *sz_before < node->vector.tokens[*i].sz; *sz_before += 1){
        if (strchr("\'\"`", node->vector.tokens[*i].str[*sz_before]))
            move = &node->vector.tokens[*i].str[*sz_before];
        if (move)
            return move;
    }
    return move;
}

static
bool check_quotes(ast_t *node, size_t *i, exec_ctx_t *ctx, args_t *args)
{
    size_t sz_before = 0;
    char *move = init_move(node, i, &sz_before);
    char be_matched;

    if (move == NULL)
        return true;
    be_matched = move[0];
    if (node->vector.tokens[*i].sz == 1)
        return (fprintf(stderr, "Unmatched \'%c\'.\n", be_matched), true);
    memmove(&move[0], &move[1], node->vector.tokens[*i].sz - sz_before);
    if (!format_quotes(node, be_matched, i))
        return true;
    if (be_matched == '`')
        return handle_magic_quotes(node, ctx, i, args);
    if (be_matched == '\"')
        return handle_var(node, i, ctx, args);
    args->args[args->sz] = strdup(node->vector.tokens[*i].str);
    return false;
}

bool check_for_closable(ast_t *node, exec_ctx_t *ctx, size_t *i, args_t *args)
{
    for (size_t j = 0; j < node->vector.tokens[*i].sz; j++){
        if (strchr("\'\"`()", node->vector.tokens[*i].str[j]))
            break;
        if (j + 1 == node->vector.tokens[*i].sz)
            return false;
    }
    if (!check_parentheses(node, i, ctx, args))
        return true;
    if (!check_quotes(node, i, ctx, args))
        return true;
    args->args[args->sz] = nullptr;
    return true;
}

void handle_var_case(ast_t *node, exec_ctx_t *ctx, size_t *i, args_t *args)
{
    if (check_for_closable(node, ctx, i, args))
        return;
    handle_var(node, i, ctx, args);
}
