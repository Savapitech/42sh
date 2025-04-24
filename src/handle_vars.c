/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** handle_vars
*/

#include <stdio.h>

#include "ast.h"
#include "env.h"

char *handle_var_case(ast_t *node, size_t *i, env_t *env)
{
    char *r_char;

    if (node->vector.tokens[*i].type == T_VAR && *i + 1 < node->vector.sz) {
        (*i)++;
        node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
        r_char = get_env_value(env, node->vector.tokens[*i].str);
        if (r_char == NULL) {
            printf("%s: Undefined variable.\n",
                node->vector.tokens[*i].str);
            return NULL;
        }
        return r_char;
    }
    node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
    return node->vector.tokens[*i].str;
}
