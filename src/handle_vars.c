/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** handle_vars
*/

#include "ast.h"
#include "env.h"
#include "stdio.h"
#include "local.h"

char *handle_var_case(ast_t *node, exec_ctx_t *ctx, size_t *i)
{
    char *r_char;

    if (node->vector.tokens[*i].type == T_VAR && *i + 1 < node->vector.sz) {
        *i += 1;
        node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
        r_char = get_env_value(ctx->env, node->vector.tokens[*i].str);
        if (r_char == NULL)
            r_char = get_local_value(ctx->local, node->vector.tokens[*i].str);
        if (r_char == NULL) {
            printf("%s: Undefined variable.\n", node->vector.tokens[*i].str);
            return NULL;
        }
        return r_char;
    }
    node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
    return node->vector.tokens[*i].str;
}
