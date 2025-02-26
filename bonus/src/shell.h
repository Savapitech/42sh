/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef SHELL_H
    #define SHELL_H
    #include "vt100_esc_codes.h"
    #define SHELL_PROMPT RED "|> " RESET

typedef struct {
    char **cmd_history;
    int last_exit_code;
    char *last_chdir;
} history_t;
int shell(char **env);
#endif /* SHELL_H */
