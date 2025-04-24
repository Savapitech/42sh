/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** handle_vars
*/

#include "ast.h"
#include "env.h"
#include "stdio.h"

char *handle_var_case(token_t *tok, size_t sz, size_t i, env_t *env)
{
    char *r_char;

    if (tok->type == T_VAR && i + 1 < sz) {
        tok++;
        tok->str[tok->sz] = '\0';
        r_char = get_env_value(env, tok->str);
        if (r_char == NULL) {
            printf("%s: Undefined variable.\n", tok->str);
            return NULL;
        }
        return r_char;
    }
    tok->str[tok->sz] = '\0';
    return tok->str;
}
