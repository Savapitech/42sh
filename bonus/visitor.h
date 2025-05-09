/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef VISITOR_H
    #define VISITOR_H
    #include "ast.h"
    #include "exec.h"


// Main
int visit_expression(ef_t *ef, ast_t *node);


// List (pipe, ...)
int visit_list(ef_t *ef, ast_t *node);

// Conditions
int visit_if(ef_t *ef, ast_t *node);
int visit_and(ef_t *ef, ast_t *node);
int visit_or(ef_t *ef, ast_t *node);
int visit_condition(ef_t *ef, ast_t *node);
#endif /* VISITOR_H */
