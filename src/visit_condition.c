/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <unistd.h>

#include "ast.h"
#include "common.h"
#include "debug.h"
#include "exec.h"
#include "u_str.h"
#include "visitor.h"

int visit_and(ef_t *ef, ast_t *node)
{
    int result = RETURN_FAILURE;

    if (!node->binary.left || !node->binary.right)
        return WRITE_CONST(STDERR_FILENO, "Invalid null l/r command.\n"),
            RETURN_FAILURE;
    result = visit_condition(ef, node->binary.left);
    if (!result)
        result = visit_condition(ef, node->binary.right);
    return result;
}

int visit_or(ef_t *ef, ast_t *node)
{
    int result = RETURN_FAILURE;

    if (!node->binary.left || !node->binary.right)
        return WRITE_CONST(STDERR_FILENO, "Invalid null l/r command.\n"),
            RETURN_FAILURE;
    result = visit_condition(ef, node->binary.left);
    if (result)
        result = visit_condition(ef, node->binary.right);
    return result;
}

static
int visit_then(ef_t *ef, ast_t *node)
{
    int result = RETURN_FAILURE;

    for (size_t i = 0; i < node->cond.sz; i++)
        result = visit_expression(ef, node->cond.nodes[i]);
    return result;
}

static
int visit_else(ef_t *ef, ast_t *node)
{
    int result = RETURN_FAILURE;

    for (size_t i = 0; i < node->cond.sz2; i++)
        result = visit_expression(ef, node->cond.nodes2[i]);
    return result;
}

int visit_if(ef_t *ef, ast_t *node)
{
    int result = RETURN_FAILURE;

    if (node->cond.sz < 1)
        return WRITE_CONST(STDERR_FILENO, "Empty if.\n"),
            RETURN_FAILURE;
    result = visit_expression(ef, node->cond.exp);
    U_DEBUG("If exp result [%d]\n", result);
    if (result == RETURN_FAILURE)
        return visit_else(ef, node);
    return visit_then(ef, node);
}
