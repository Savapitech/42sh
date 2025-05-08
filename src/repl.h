/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef REPL_H
    #define REPL_H

    #include <stdio.h>

    #include "builtins_handler.h"
    #include "u_str.h"

void init_shell_repl(exec_ctx_t *exec_ctx);

void restore_term_flags(exec_ctx_t *exec_ctx);

ssize_t handle_keys(
    exec_ctx_t *ec,
    buff_t *buff,
    char const *read_buff,
    size_t len);

void print_shell_prompt(exec_ctx_t *exec_ctx);
#endif /* REPL_H */
