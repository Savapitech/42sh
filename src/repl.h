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
    #include "readline.h"
    #include "u_str.h"

void init_shell_repl(exec_ctx_t *ec);

void restore_term_flags(exec_ctx_t *exec_ctx);

ssize_t handle_keys(
    readline_helper_t *rh,
    buff_t *buff,
    char const *read_buff,
    size_t len);

void print_shell_prompt(exec_ctx_t *ec);
void print_second_shell_prompt(exec_ctx_t *ec);
bool is_sequence(char *read_buff);
#endif /* REPL_H */
