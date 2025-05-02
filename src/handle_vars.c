/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** handle_vars
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>

#include "ast.h"
#include "env.h"
#include "local.h"
#include "exec.h"

static
char *handle_variable(ast_t *node, exec_ctx_t *ctx, size_t *i)
{
    char *r_char;

    (*i)++;
    node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
    r_char = get_env_value(ctx->env, node->vector.tokens[*i].str);
    if (r_char == NULL)
        r_char = get_local_value(ctx->local, node->vector.tokens[*i].str);
    if (r_char == NULL) {
        fprintf(stderr, "%s: Undefined variable.\n",
            node->vector.tokens[*i].str);
        return NULL;
    }
    return r_char;
}

static
char *take_next_parenthese_arg(ast_t *node, size_t *in_str, size_t *i)
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
bool handle_parentheses(ast_t *node, exec_ctx_t *ctx, size_t *i, args_t *args)
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
    args->args[args->sz] = NULL;
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
    if (!handle_parentheses(node, ctx, i, args))
        return true;
    return false;
}

static
bool check_quotes(ast_t *node, size_t *i, exec_ctx_t *ctx, args_t *args)
{
    char be_matched = node->vector.tokens[*i].str[0];

    if (!strchr("\'\"`", node->vector.tokens[*i].str[0]))
        return true;
    if (strlen(node->vector.tokens[*i].str) == 1 || be_matched !=
        node->vector.tokens[*i].str[node->vector.tokens[*i].sz - 1])
        return (fprintf(stderr, "Unmatched \'%c\'.\n", be_matched), true);
    node->vector.tokens[*i].str[node->vector.tokens[*i].sz - 1] = '\0';
    if (be_matched == '`'){
        handle_magic_quotes(node, ctx, i, args);
        return false;
    }
    args->args[args->sz] = strdup(&node->vector.tokens[*i].str[1]);
    return false;
}

bool check_for_closable(ast_t *node, exec_ctx_t *ctx, size_t *i, args_t *args)
{
    if (!strchr("\'\"`()", node->vector.tokens[*i].str[0]))
        return false;
    if (!check_parentheses(node, i, ctx, args))
        return true;
    else if (!check_quotes(node, i, ctx, args))
        return true;
    args->args[args->sz] = NULL;
    return true;
}

void handle_var_case(ast_t *node, exec_ctx_t *ctx, size_t *i, args_t *args)
{
    if (node->vector.tokens[*i].type == T_VAR && *i + 1 < node->vector.sz){
        args->args[args->sz] = handle_variable(node, ctx, i);
        return;
    }
    if (check_for_closable(node, ctx, i, args))
        return;
    args->args[args->sz] = strndup(node->vector.tokens[*i].str,
        node->vector.tokens[*i].sz);
}
