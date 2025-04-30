/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <string.h>

#include "expr.h"

static
expr_val_t apply_add(expr_val_t *left, expr_val_t *right)
{
    expr_val_t res;

    if (left->type != E_VAL_INT || right->type != E_VAL_INT) {
        res.str = "non-integer argument";
        res.type = E_VAL_ERR;
    } else {
        res.val = left->val + right->val;
        res.type = E_VAL_INT;
    }
    return res;
}

static
expr_val_t apply_sub(expr_val_t *left, expr_val_t *right)
{
    expr_val_t res;

    if (left->type != E_VAL_INT || right->type != E_VAL_INT) {
        res.str = "non-integer argument";
        res.type = E_VAL_ERR;
    } else {
        res.val = left->val - right->val;
        res.type = E_VAL_INT;
    }
    return res;
}

static
expr_val_t apply_mul(expr_val_t *left, expr_val_t *right)
{
    expr_val_t res;

    if (left->type != E_VAL_INT || right->type != E_VAL_INT) {
        res.str = "non-integer argument";
        res.type = E_VAL_ERR;
    } else {
        res.val = left->val * right->val;
        res.type = E_VAL_INT;
    }
    return res;
}

static
expr_val_t apply_div(expr_val_t *left, expr_val_t *right)
{
    expr_val_t res = { .type = E_VAL_ERR };

    if (left->type != E_VAL_INT || right->type != E_VAL_INT) {
        res.str = "non-integer argument";
        res.type = E_VAL_ERR;
        return res;
    }
    if (right->val == 0) {
        res.str = "division by zero";
        res.type = E_VAL_ERR;
        return res;
    }
    res.val = left->val / right->val;
    res.type = E_VAL_INT;
    return res;
}

static
expr_val_t apply_lt(expr_val_t *left, expr_val_t *right)
{
    expr_val_t res = { .type = E_VAL_INT };

    if (left->type == E_VAL_INT && right->type == E_VAL_INT)
        res.val = left->val < right->val;
    else
        res.val = strcmp(left->p, right->p) < 0;
    return res;
}

static
expr_val_t apply_gt(expr_val_t *left, expr_val_t *right)
{
    expr_val_t res = { .type = E_VAL_INT };

    if (left->type == E_VAL_INT && right->type == E_VAL_INT)
        res.val = left->val > right->val;
    else
        res.val = strcmp(left->p, right->p) > 0;
    return res;
}

const expr_op_precedence_t OPERATOR_PRECEDENCE[] = {
    { .name = "+", .prec = 2, apply_add },
    { .name = "-", .prec = 2, apply_sub },
    { .name = "*", .prec = 3, apply_mul },
    { .name = "/", .prec = 3, apply_div },
    { .name = "<", .prec = 1, apply_lt },
    { .name = ">", .prec = 1, apply_gt },
};

const size_t OPERATOR_PRECEDENCE_COUNT = COUNT_OF(OPERATOR_PRECEDENCE);
