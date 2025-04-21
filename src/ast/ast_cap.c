/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdbool.h>

#include "ast.h"
#include "u_mem.h"

bool ensure_node_cap(ast_t *node)
{
    token_t *temp;

    if (node->vector.sz + 1 < node->vector.cap)
        return true;
    temp = u_realloc(node->vector.tokens,
        sizeof *node->vector.tokens * node->vector.sz,
        sizeof *node->vector.tokens * node->vector.cap << 1);
    if (temp == NULL)
        return false;
    node->vector.cap <<= 1;
    node->vector.tokens = temp;
    return true;
}

bool ensure_list_cap(ast_t *node)
{
    ast_t **temp;

    if (node->list.sz + 1 < node->vector.cap)
        return true;
    temp = (ast_t **)u_realloc((void *)node->list.nodes,
        sizeof *node->list.nodes * node->list.sz,
        sizeof *node->list.nodes * node->vector.cap << 1);
    if ((void *)temp == NULL)
        return false;
    node->list.cap <<= 1;
    node->list.nodes = temp;
    return true;
}

bool ensure_cond_cap(ast_t *node)
{
    ast_t **temp;

    if (node->cond.sz + 1 < node->vector.cap)
        return true;
    temp = (ast_t **)u_realloc((void *)node->cond.nodes,
        sizeof *node->cond.nodes * node->cond.sz,
        sizeof *node->cond.nodes * node->vector.cap << 1);
    if ((void *)temp == NULL)
        return false;
    node->cond.cap <<= 1;
    node->cond.nodes = temp;
    return true;
}
