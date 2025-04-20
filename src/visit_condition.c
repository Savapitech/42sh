/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <unistd.h>

#include "ast.h"
#include "common.h"
#include "exec.h"
#include "u_str.h"
#include "visitor.h"

int visit_and(ef_t *ef, ast_t *node)
{
    int result = RETURN_FAILURE;

    if (!node->binary.left || !node->binary.right)
        return WRITE_CONST(STDERR_FILENO, "Invalid null l/r command.\n"),
            RETURN_FAILURE;
    result = visit_list(ef, node->binary.left);
    if (!result)
        result = visit_list(ef, node->binary.right);
    return result;
}

int visit_or(ef_t *ef, ast_t *node)
{
    int result = RETURN_FAILURE;

    if (!node->binary.left || !node->binary.right)
        return WRITE_CONST(STDERR_FILENO, "Invalid null l/r command.\n"),
            RETURN_FAILURE;
    result = visit_list(ef, node->binary.left);
    if (result)
        result = visit_list(ef, node->binary.right);
    return result;
}
