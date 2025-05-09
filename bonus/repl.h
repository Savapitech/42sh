/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef REPL_H
    #define REPL_H
    #include "exec.h"
    #include "u_str.h"

void init_shell_repl(exec_ctx_t *exec_ctx);
void restore_term_flags(exec_ctx_t *exec_ctx);
bool handle_keys(exec_ctx_t *exec_ctx, buff_t *buff, char const *read_buff);
void print_shell_prompt(exec_ctx_t *exec_ctx);
void ignore_sigint(exec_ctx_t *exec_ctx);
#endif /* REPL_H */
