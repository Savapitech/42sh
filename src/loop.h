/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef LOOP_H
    #define LOOP_H
    #include <stdbool.h>
    #include <stddef.h>

    #include "exec.h"

typedef struct {
    size_t sz;
    size_t cap;
    char **cmds;
} usr_cmd_t;

usr_cmd_t *get_usr_loop_cmd(exec_ctx_t *exec_ctx, usr_cmd_t *usr_cmd,
    char const *prompt);
#endif /* LOOP_H */
