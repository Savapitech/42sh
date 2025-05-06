/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** handle_quotes
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast.h"
#include "exec.h"

static
char *set_f_part(char *node_str)
{
    char *f_part = strdup(node_str);
    int i = 0;

    if (f_part == NULL)
        return nullptr;
    for (; f_part[i] && f_part[i] != '$'; i++);
    f_part[i] = '\0';
    return f_part;
}

static
char *set_s_part(char *node_str, int len_f_part, ast_t *node, const size_t *i)
{
    size_t pointer_rank = len_f_part;

    for (; pointer_rank < node->vector.tokens[*i].sz && node_str[pointer_rank]
        && !isblank(node_str[pointer_rank]); pointer_rank++);
    return strndup(&node_str[pointer_rank],
        node->vector.tokens[*i].sz - pointer_rank);
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
    free(s_part);
    free(f_part);
    return false;
}

static
bool concat_var(char *var, args_t *args, ast_t *node, size_t *i)
{
    char *f_part = set_f_part(node->vector.tokens[*i].str);
    char *s_part = nullptr;

    args->args[args->sz] = var;
    if (f_part == NULL)
        return true;
    s_part = set_s_part(node->vector.tokens[*i].str, strlen(f_part), node, i);
    if (s_part == NULL)
        return free(f_part), true;
    return concat_and_free(var, f_part, s_part, args);
}

static
char *get_key(ast_t *node, const size_t *i)
{
    size_t id = 0;

    for (; node->vector.tokens[*i].str &&
        id < node->vector.tokens[*i].sz; id++)
        if (node->vector.tokens[*i].str[id] == '$'){
            id++;
            return strndup(&node->vector.tokens[*i].str[id],
                node->vector.tokens[*i].sz - id);
            }
    return nullptr;
}

bool handle_var(ast_t *node, size_t *i, exec_ctx_t *ctx, args_t *args)
{
    char *key = get_key(node, i);
    char *var = nullptr;
    int end_key = 0;

    if (key == NULL){
        args->args[args->sz] = strndup(node->vector.tokens[*i].str,
            node->vector.tokens[*i].sz);
        return false;
    }
    if (key == NULL)
        return true;
    for (; key[end_key] && !isblank(key[end_key]); end_key++);
    key[end_key] = '\0';
    var = get_values(ctx, key);
    free(key);
    if (var == NULL)
        return true;
    return concat_var(var, args, node, i);
}
