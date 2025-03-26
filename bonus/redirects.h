/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef REDIRECTS_H
    #define REDIRECTS_H
    #include <stdbool.h>

    #include "exec.h"

bool handle_out_redirect(ef_t *ef, ast_t *node, size_t i, size_t sz);
bool handle_in_redirect(ef_t *ef, ast_t *node, size_t i, size_t sz);
int handle_heredoc_loop(ast_t *node, size_t i);
bool handle_heredoc(ef_t *ef, ast_t *node, size_t i, size_t sz);
#endif /* REDIRECTS_H */
