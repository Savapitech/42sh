/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef HISTORY_H
    #define HISTORY_H

    #include <stddef.h>

#define CHAR_HIST '!'
#define TWO_CHAR_CMD 3

typedef struct history_variable_s {
        int coord_variable;
        int size_variable;
        int type;
} his_variable_t;

typedef struct history_command_s {
        int id;
        char time[5];// []h[]
        char *command;
        char *arg;
        int sz;
} his_command_t;
    
typedef struct parsing_history_s {
        char *name;
        char *(*funct)(char *, his_variable_t *, his_command_t *);
} parsing_history_t;

char *parse_history(char *line, size_t *buffer);
char *his_last_command(char *line, his_variable_t *his_variable, his_command_t *his_command);
char *his_last_same_command(char *line, his_variable_t *his_variable, his_command_t *his_command);
char *his_id_command(char *line, his_variable_t *his_variable, his_command_t *his_command);
char *his_last_word(char *line, his_variable_t *his_variable, his_command_t *his_command);
char *his_last_arg(char *line, his_variable_t *his_variable, his_command_t *his_command);
#endif /* HISTORY_H */


