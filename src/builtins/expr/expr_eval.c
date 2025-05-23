/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr.h"

static
const expr_op_precedence_t *op_precedence(char *op)
{
    for (size_t i = 0; i < OPERATOR_PRECEDENCE_COUNT; i++)
        if (!strcmp(op, OPERATOR_PRECEDENCE[i].name))
            return OPERATOR_PRECEDENCE + i;
    return nullptr;
}

static
expr_val_t expr_parse_paren(expr_state_t *state, uchar_t depth)
{
    expr_val_t out;

    state->args++;
    out = expr_run(state, depth, 0);
    if (out.type == E_VAL_ERR)
        return out;
    if (*state->args == NULL || !IS_CHAR(*state->args, ')')) {
        out.str = "syntax error: missing ')'";
        out.type = E_VAL_ERR;
        return out;
    }
    state->args++;
    return out;
}

static
void expr_solve_precedence(
    expr_val_t *out,
    expr_state_t *state, uchar_t depth, unsigned int prec)
{
    const expr_op_precedence_t *op_prec;
    expr_val_t tmp;

    while (*state->args != NULL && !IS_CHAR(*state->args, ')')) {
        op_prec = op_precedence(*state->args);
        if (op_prec == NULL) {
            out->type = E_VAL_ERR;
            out->str = "syntax error: unexpected argument";
            break;
        }
        if (op_prec->prec == 0 || op_prec->prec <= prec)
            return;
        state->args++;
        tmp = expr_run(state, depth, op_prec->prec);
        if (tmp.type == E_VAL_ERR) {
            *out = tmp;
            break;
        }
        *out = op_prec->apply(out, &tmp);
    }
}

expr_val_t expr_parse_val(expr_state_t *state)
{
    expr_val_t out;
    char *chk;

    out.val = strtol(*state->args, &chk, 10);
    out.p = *state->args;
    if (chk != *state->args && *chk == '\0')
        out.type = E_VAL_INT;
    else
        out.type = E_VAL_STR;
    state->args++;
    return out;
}

expr_val_t expr_run(expr_state_t *state, uchar_t depth, int prec)
{
    expr_val_t out = {
        .type = E_VAL_ERR,
        .str = "syntax error: missing argument"
    };

    if (*state->args == NULL)
        return out;
    if (IS_CHAR(*state->args, '('))
        out = expr_parse_paren(state, depth + 1);
    else
        out = expr_parse_val(state);
    expr_solve_precedence(&out, state, depth, prec);
    return out;
}
