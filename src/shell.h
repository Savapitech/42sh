/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef SHELL_H
    #define SHELL_H
    #include "common.h"

typedef struct {
    char **cmd_history;
    int last_exit_code;
    char *last_chdir;
} history_t;

int shell(opt_t *opt, char **env);
#endif /* SHELL_H */
