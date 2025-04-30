/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** magic_quotes
*/

#include <glob.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "exec.h"
#include "u_mem.h"

static
void exec_magic(ast_t *node, exec_ctx_t *ctx, size_t *i, int fd[2])
{
    pid_t pid = 0;

    node->vector.tokens[*i].str[node->vector.tokens[*i].sz - 1] = '\0';
    memmove(&node->vector.tokens[*i].str[0],
        &node->vector.tokens[*i].str[1], strlen(node->vector.tokens[*i].str));
    pid = fork();
    if (pid == 0){
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        visitor(node->vector.tokens[*i].str, ctx);
        close(fd[1]);
        exit(ctx->history->last_exit_code);
    }
    wait(&ctx->history->last_exit_code);
}

char *realloc_buffer(char *to_return, size_t *sz, size_t *i)
{
    char *temp;

    if (to_return == NULL){
        *sz = 2;
        *i = 0;
        to_return = malloc(sizeof(char) * *sz);
    }
    if (*i >= *sz){
        temp = realloc(to_return, *sz << 1);
        *sz = *sz << 1;
        if (temp == NULL)
            return free(to_return), NULL;
        to_return = temp;
    }
    return to_return;
}

char *add_to_args(char *to_return, args_t *args, size_t *sz, size_t i)
{
    to_return = realloc_buffer(to_return, sz, &i);
    if (to_return == NULL)
        return NULL;
    to_return[i] = '\0';
    for (int j = 0; to_return[j]; j++)
        if (to_return[j] == '\n')
            to_return = &to_return[j];
    if (!ensure_args_capacity(args))
        return (free(to_return), NULL);
    args->args[args->sz] = to_return;
    args->sz++;
    return NULL;
}

char *handle_buffer(char *to_return, size_t *sz, size_t *i, char buf)
{
    to_return = realloc_buffer(to_return, sz, i);
    if (to_return == NULL)
        return NULL;
    to_return[*i] = buf;
    return to_return;
}

static
bool skip_first_output(char **to_return, int *skip_first, char buf)
{
    if (buf == '\n' && !*skip_first){
        *skip_first = 1;
        free(*to_return);
        *to_return = NULL;
        return true;
    }
    return false;
}

static
void put_output(int fd[2], args_t *args, char *to_return, size_t sz)
{
    char buf;
    int skip_first = 0;

    for (size_t i = 0; read(fd[0], &buf, 1) > 0; i++){
        if (strchr("$'", buf)){
            i--;
            continue;
        }
        if (buf == '\n' && skip_first){
            to_return = add_to_args(to_return, args, &sz, i);
            continue;
        }
        if (skip_first_output(&to_return, &skip_first, buf))
            continue;
        to_return = handle_buffer(to_return, &sz, &i, buf);
    }
    if (to_return != NULL)
        free(to_return);
}

static
void get_output(int fd[2], args_t *args)
{
    size_t sz = 2;
    char *to_return = malloc(sizeof(char) * sz);

    if (to_return == NULL)
        return;
    put_output(fd, args, to_return, sz);
    args->sz -= 2;
}

char *handle_magic_quotes(ast_t *node, exec_ctx_t *ctx,
    size_t *i, args_t *args)
{
    char *to_return = 0;
    int fd[2];

    if (pipe(fd) == -1)
        return NULL;
    exec_magic(node, ctx, i, fd);
    close(fd[1]);
    get_output(fd, args);
    close(fd[0]);
    return to_return;
}
