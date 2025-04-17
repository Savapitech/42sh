/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** handle_vars
*/

#include "ast.h"
#include "common.h"
#include "debug.h"
#include "env.h"
#include "history.h"
#include "shell.h"
#include "u_str.h"

char *handle_var_case(ast_t *node, env_t *env, size_t *i)
{
    if (node->vector.tokens[*i].type == T_VAR && *i + 1 < node->vector.sz) {
        *i += 1;
        node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
        return get_env_value(env, node->vector.tokens[*i].str);
    } else {
        node->vector.tokens[*i].str[node->vector.tokens[*i].sz] = '\0';
        return node->vector.tokens[*i].str;
    }
}