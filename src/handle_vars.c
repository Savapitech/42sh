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
bool check_parentheses(ast_t *node, size_t *i, args_t *args)
{
    if (!strchr("()", node->vector.tokens[*i].str[0]))
        return true;
    if (node->vector.tokens[*i].str[0] == '(' &&
        node->vector.tokens[*i].str[node->vector.tokens[*i].sz] != ')')
        return (fprintf(stderr, "Too many ('s.\n"), true);
    if (node->vector.tokens[*i].str[0] == ')')
        return (fprintf(stderr, "Too many )'s.\n"), true);
    node->vector.tokens[*i].str[node->vector.tokens[*i].sz - 1] = '\0';
    if (isblank(&node->vector.tokens[*i].str[1]))
        return (fprintf(stderr, "Invalid null command.\n"), true);
    args->args[args->sz] = strdup(&node->vector.tokens[*i].str[1]);
    return false;
}

static
bool check_quotes(ast_t *node, size_t *i, exec_ctx_t *ctx, args_t *args)
{
    char be_matched = node->vector.tokens[*i].str[0];

    if (!strchr("\'\"`", node->vector.tokens[*i].str[0]))
        return true;
    if (be_matched !=
        node->vector.tokens[*i].str[node->vector.tokens[*i].sz - 1]
        || strlen(node->vector.tokens[*i].str) == 1)
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
    if (!check_parentheses(node, i, args))
        return true;
    else if (!check_quotes(node, i, ctx, args))
        return true;
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
    node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
    args->args[args->sz] = node->vector.tokens[*i].str;
}
