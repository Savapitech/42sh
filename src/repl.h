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
bool handle_keys(buff_t *buff, char *read_buff);
#endif /* REPL_H */
