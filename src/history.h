/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef HISTORY_H
    #define HISTORY_H
    #define CHAR_HIST '!'
    #define TWO_CHAR_CMD 3

    #include <stddef.h>



typedef struct history_variable_s {
    int coord_variable;
    int size_variable;
    int type;
    char *str;
    int id;
} his_variable_t;

typedef struct history_command_s {
    int id;
    char *command;
    char *arg;
    int sz;
} his_command_t;

typedef struct parsing_history_s {
    char const *name;
    char *(*funct)(char *, his_variable_t *, his_command_t *);
} parsing_history_t;

extern const parsing_history_t tab_fnct[];

int parse_history(char **pointer_line,
    size_t *buffer_len, size_t *buffer_sz, his_command_t **cmd_history);
char *his_last_command(char *line,
    his_variable_t *his_variable, his_command_t *his_command);
char *his_last_same_command(char *line,
    his_variable_t *his_variable, his_command_t *his_command);
char *his_id_command(char *line,
    his_variable_t *his_variable, his_command_t *his_command);
char *his_last_word(char *line,
    his_variable_t *his_variable, his_command_t *his_command);
char *his_last_arg(char *line,
    his_variable_t *his_variable, his_command_t *his_command);
his_command_t *fill_cmd_history(his_command_t *cmd_history);
int save_cmd_history(his_command_t *cmd_history);
his_command_t set_cmd(char *line, his_command_t cmd_struct);
#endif /* HISTORY_H */
