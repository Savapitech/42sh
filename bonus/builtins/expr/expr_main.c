/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "exec.h"
#include "expr.h"

int builtins_expr(ef_t *ef[[gnu::unused]], char **args)
{
    int argc = 0;
    expr_state_t state;
    expr_val_t ret;

    for (; args[argc] != nullptr; argc++);
    if (argc < 2)
        return fprintf(stderr, "%s: missing operand\nTry 'expr --help' for"
            " more information.", args[0]), -1;
    state = (expr_state_t){ .args = &args[1] };
    ret = expr_run(&state, 0, 0);
    if (ret.type == E_VAL_ERR)
        return printf("%s: %s\n", args[0], ret.str), -1;
    if (ret.type == E_VAL_INT && (strcmp("if", args[0]) == 0 ||
        strcmp("while", args[0]) == 0))
        return ret.val;
    if (ret.type == E_VAL_INT)
        printf("%ld\n", ret.val);
    if (ret.type == E_VAL_STR)
        printf("%s\n", ret.p);
    return RETURN_SUCCESS;
}
