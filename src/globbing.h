/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** globbing
*/

#ifndef GLOBBING_H
    #define GLOBBING_H
    #include <glob.h>
    #include <stdbool.h>

    #include "ast.h"
    #include "exec.h"

typedef struct globs_s {
    glob_t globs;
    char **found_tab;
    char **result_tab;
    int val;
}globs_t;

bool process_args(ast_t *node, args_t *args, size_t *toks_i, ef_t *ef);
#endif /* GLOBBING_H */
