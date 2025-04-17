/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** get_usr_loop_cmd
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "redirects.h"
#include "u_str.h"
#include "u_mem.h"

static
ast_t *buffers_realloc(ast_t *node)
{
    char **new_buffers = u_realloc(node->loop.buffers, sizeof
        *node->loop.buffers * node->loop.sz, sizeof
        *node->loop.buffers * (node->loop.cap << 1));

    if (new_buffers == NULL)
        return NULL;
    node->loop.buffers = new_buffers;
    node->loop.cap <<= 1;
    return node;
}

static
ast_t *get_first_cmd(ast_t *node, char prompt[], size_t *bf_len)
{
    size_t buffer_len = 0;

    if (isatty(STDIN_FILENO))
        printf("%s? ", prompt);
    node->loop.buffers[node->loop.sz] = NULL;
    getline(&(node->loop.buffers[node->loop.sz]), bf_len, stdin);
    if (!feof(stdin)){
        buffer_len = u_strlen(node->loop.buffers[node->loop.sz]);
        node->loop.buffers[node->loop.sz][buffer_len - 1] = '\0';
    }
    if (feof(stdin)){
        free(node->loop.buffers[node->loop.sz]);
        node->loop.buffers[node->loop.sz] = NULL;
    }
    node->loop.sz += 1;
    return node;
}

static
ast_t *increase_buffers(ast_t *node, size_t *buffer_len)
{
    node->loop.buffers[node->loop.sz] = NULL;
    getline(&(node->loop.buffers[node->loop.sz]), buffer_len, stdin);
    if (!feof(stdin)){
        *buffer_len = u_strlen(node->loop.buffers[node->loop.sz]);
        node->loop.buffers[node->loop.sz][*buffer_len - 1] = '\0';
    }
    if (feof(stdin)){
        free(node->loop.buffers[node->loop.sz]);
        node->loop.buffers[node->loop.sz] = NULL;
    }
    node->loop.sz++;
    return node;
}

static
ast_t *handle_end(ast_t *node, char prompt[])
{
    node->loop.sz--;
    if (!node->loop.buffers[node->loop.sz] ||
        strcmp("end", node->loop.buffers[node->loop.sz])){
        printf("%s: end not found.\n", prompt);
        free_array(node->loop.buffers);
        node->loop.buffers = NULL;
        exit(RETURN_FAILURE);
        return NULL;
    }
    free(node->loop.buffers[node->loop.sz]);
    node->loop.buffers[node->loop.sz] = NULL;
    return node;
}

ast_t *get_usr_loop_cmd(ast_t *node)
{
    char prompt[] = "foreach";
    size_t buffer_sz;

    if (node->tok.type == T_WHILE)
        strcpy(prompt, "while");
    node = get_first_cmd(node, prompt, &buffer_sz);
    while (!feof(stdin) &&
    strcmp("end", node->loop.buffers[node->loop.sz - 1])){
        if (isatty(STDIN_FILENO))
            printf("%s? ", prompt);
        if (node->loop.sz >= node->loop.cap)
            node = buffers_realloc(node);
        if (node == NULL)
            return NULL;
        increase_buffers(node, &buffer_sz);
    }
    node = handle_end(node, prompt);
    return node;
}
