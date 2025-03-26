/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exec.h"
#include "u_str.h"

bool handle_out_redirect(ef_t *ef, ast_t *node, size_t i, size_t sz)
{
    if (!(node->vector.tokens[i].type & (T_REDIRECT | T_APPEND)))
        return true;
    if (i >= sz || node->vector.tokens[i + 1].type != T_ARG)
        return (WRITE_CONST(STDERR_FILENO,
            "Missing name for redirect.\n"), false);
    ef->skip_i = ef->skip_i ?: i;
    ef->skip_sz += 2;
    node->vector.tokens[i + 1].str[node->vector.tokens[i + 1].sz] = '\0';
    ef->rout_fd = open(node->vector.tokens[i + 1].str, O_CREAT | O_WRONLY |
        (node->vector.tokens[i].type == T_APPEND ? O_APPEND : O_TRUNC), 0644);
    if (ef->rout_fd < 0)
        return (puterror(node->vector.tokens[i + 1].str), false);
    ef->out_fd = ef->rout_fd;
    return true;
}

bool handle_in_redirect(ef_t *ef, ast_t *node, size_t i, size_t sz)
{
    if (node->vector.tokens[i].type != T_IN_REDIRECT)
        return true;
    if (i >= sz || node->vector.tokens[i + 1].type != T_ARG)
        return (WRITE_CONST(STDERR_FILENO,
            "Missing name for redirect.\n"), false);
    ef->skip_i = ef->skip_i ?: i;
    ef->skip_sz += 2;
    node->vector.tokens[i + 1].str[node->vector.tokens[i + 1].sz] = '\0';
    ef->rin_fd = open(node->vector.tokens[i + 1].str, O_RDONLY);
    if (ef->rin_fd < 0)
        return (puterror(node->vector.tokens[i + 1].str), false);
    ef->in_fd = ef->rin_fd;
    return true;
}

int handle_heredoc_loop(ast_t *node, size_t i)
{
    int fds[2];
    buff_t buffer = { .str = NULL };

    if (pipe(fds) < 0)
        return (puterror("pipe"), -1);
    node->vector.tokens[i + 1].str[node->vector.tokens[i + 1].sz] = '\0';
    WRITE_CONST(STDOUT_FILENO, "? ");
    while (getline(&buffer.str, &buffer.sz, stdin) != -1) {
        buffer.str[u_strlen(buffer.str) - 1] = '\0';
        if (u_strcmp(buffer.str, node->vector.tokens[i + 1].str) == 0)
            break;
        write(fds[1], buffer.str, u_strlen(buffer.str));
        write(fds[1], "\n", 1);
        WRITE_CONST(STDOUT_FILENO, "? ");
    }
    free(buffer.str);
    close(fds[1]);
    return fds[0];
}

bool handle_heredoc(ef_t *ef, ast_t *node, size_t i, size_t sz)
{
    if (node->vector.tokens[i].type != T_HEREDOC)
        return true;
    if (i >= sz || node->vector.tokens[i + 1].type != T_ARG)
        return (WRITE_CONST(STDERR_FILENO,
            "Missing name for redirect.\n"), false);
    ef->skip_i = ef->skip_i ?: i;
    ef->skip_sz += 2;
    ef->in_fd = handle_heredoc_loop(node, i);
    if (ef->in_fd == -1)
        return false;
    return true;
}
