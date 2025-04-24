/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** globbing
*/

#include <glob.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exec.h"
#include "globbing.h"
#include "u_str.h"

#include "debug.h"

bool check_glob_result(int val, char *bin_name)
{
    if (val != 0){
        if (val == GLOB_NOMATCH)
            dprintf(STDERR_FILENO, "%s; No match.\n", bin_name);
        return false;
    }
    return true;
}

bool process_globbing(char *pattern, args_t *args)
{
    glob_t globs;
    int glob_result;

    glob_result = glob(pattern, GLOB_ERR, NULL, &globs);
    if (!check_glob_result(glob_result, args->args[0]))
        return false;
    for (size_t i = 0; i < globs.gl_pathc; i++) {
        ensure_args_capacity(args);
        args->args[args->sz] = strdup(globs.gl_pathv[i]);
        if (args->args[args->sz] == NULL)
            return globfree(&globs), false;
        args->sz++;
    }
    globfree(&globs);
    return true;
}

bool process_args(ast_t *node, args_t *args, size_t toks_i, ef_t *ef)
{
    token_t tok = node->vector.tokens[toks_i];

    U_DEBUG("tok [%s]\n", tok.str);
    if (strchr(tok.str, '*') != NULL)
        return (process_globbing(tok.str, args));
    if (!ensure_args_capacity(args))
        return false;
    args->args[args->sz] = handle_var_case(node->vector.tokens,
        node->vector.sz, toks_i, ef->env);
    if (args->args[args->sz] == NULL)
        return false;
    args->sz++;
    return true;
}
