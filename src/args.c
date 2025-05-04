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

#include "args.h"
#include "exec.h"

bool check_glob_result(int val, char *bin_name)
{
    if (val != 0){
        if (val == GLOB_NOMATCH)
            dprintf(STDERR_FILENO, "%s: No match.\n", bin_name);
        return false;
    }
    return true;
}

bool process_globbing(char *pattern, args_t *args, size_t *toks_i)
{
    glob_t globs;
    int glob_result;
    char *vl;

    glob_result = glob(pattern, GLOB_ERR, nullptr, &globs);
    if (!check_glob_result(glob_result, args->args[0]))
        return false;
    for (size_t i = 0; i < globs.gl_pathc; i++) {
        ensure_args_capacity(args);
        vl = strdup(globs.gl_pathv[i]);
        if (vl == NULL)
            return globfree(&globs), false;
        args->args[args->sz] = vl;
        args->sz++;
    }
    globfree(&globs);
    *toks_i += 1;
    return true;
}

static
bool handle_tilde(ef_t *ef, args_t *args, size_t *toks_i)
{
    char *home;

    if (!ensure_args_capacity(args))
        return false;
    home = get_env_value(ef->env, "HOME");
    if (home != NULL)
        args->args[args->sz] = get_env_value(ef->env, "HOME");
    else
        args->args[args->sz] = strdup("");
    args->sz++;
    *toks_i += 1;
    return true;
}

bool process_args(ast_t *node, args_t *args, size_t *toks_i, ef_t *ef)
{
    token_t tok = node->vector.tokens[*toks_i];

    if (!ensure_args_capacity(args))
        return false;
    if (tok.type == T_STAR || strcspn(tok.str, "[]?") != strlen(tok.str))
        return (process_globbing(tok.str, args, toks_i));
    if (tok.type == T_TILDE)
        return handle_tilde(ef, args, toks_i);
    handle_var_case(node, ef->exec_ctx, toks_i, args);
    if (args->args[args->sz] == NULL)
        return false;
    args->sz++;
    return true;
}
