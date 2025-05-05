/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** handle_quotes
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>

#include "ast.h"
#include "env.h"
#include "local.h"
#include "exec.h"

static
char *set_f_part(char *node_str)
{
    char *f_part = strdup(node_str);
    int i = 0;

    if (f_part == NULL)
        return NULL;
    for (; f_part[i] && f_part[i] != '$'; i++);
    f_part[i] = '\0';
    return f_part;
}

static
char *set_s_part(char *node_str, int len_f_part)
{
    int pointer_rank = len_f_part;

    for (; node_str[pointer_rank] && !isblank(node_str[pointer_rank]);
    pointer_rank++);
    return &node_str[pointer_rank];
}

static
bool concat_and_free(char *var, char *f_part, char *s_part, args_t *args)
{
    args->args[args->sz] = malloc(sizeof(char) *
    (strlen(f_part) + strlen(s_part) + strlen(var) + 1));
    if (args->args[args->sz] == NULL)
        return free(f_part), true;
    strcpy(args->args[args->sz], f_part);
    strcat(args->args[args->sz], var);
    strcat(args->args[args->sz], s_part);
    free(f_part);
    return false;
}

static
bool concat_var(char *var, char *node_str, args_t *args)
{
    char *f_part = set_f_part(node_str);
    char *s_part = NULL;

    if (f_part == NULL)
        return true;
    s_part = set_s_part(node_str, strlen(f_part));
    if (s_part == NULL)
        return free(f_part), true;
    return concat_and_free(var, f_part, s_part, args);
}

bool handle_quotes(ast_t *node, size_t *i, exec_ctx_t *ctx, args_t *args)
{
    char *key = strchr(node->vector.tokens[*i].str, '$');
    char *var = NULL;
    int end_key = 0;

    if (key == NULL){
        args->args[args->sz] = strdup(node->vector.tokens[*i].str);
        return false;
    }
    key = strdup(&key[1]);
    if (key == NULL)
        return true;
    for (; key[end_key] && !isblank(key[end_key]); end_key++);
    key[end_key] = '\0';
    var = get_values(ctx, key);
    free(key);
    if (var == NULL)
        return true;
    args->args[args->sz] = var;
    return concat_var(var, node->vector.tokens[*i].str, args);
}
