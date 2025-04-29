/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "exec.h"
#include "expr.h"

int builtins_expr(ef_t *ef, char **args)
{
    int argc = 0;
    expr_state_t state;
    expr_val_t ret;

    for (; args[argc] != NULL; argc++);
    if (argc < 2) {
        fprintf(stderr, "expr: missing operand\n");
        return RETURN_FAILURE;
    }
    state = (expr_state_t){ .args = &args[1] };
    ret = expr_run(&state, 0, 0);
    if (ret.type == E_VAL_ERR) {
        printf("expr: %s\n", ret.str);
        return RETURN_FAILURE;
    }
    if (ret.type == E_VAL_INT)
        printf("%ld\n", ret.val);
    if (ret.type == E_VAL_STR)
        printf("%s\n", ret.p);
    return RETURN_SUCCESS;
}
