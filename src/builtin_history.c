/*
** EPITECH PROJECT, 2025
** history_42sh
** File description:
** builtin_history
*/

#include <stdlib.h>
#include <stdio.h>

#include "history.h"

/*
**Il faut deux \0 parce que dans le gettokeniser 
** y un truc qui regarde
**après le premier \0
*/

/*
**cat in str prend un
** his_variable_t en 
** parametre pour 
** connaitre la coord 
** d' ou commencer a concaténer
** mais aussi le nombre de charactère a retiré 
** il vas free le buffer
*/

static
char *cat_in_str()
{
    return NULL;
}

char *his_last_command(char *line, his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = malloc(sizeof(char) * 10);

    if (new_line == NULL)
        return NULL;
    new_line[0] = 'l';
    new_line[1] = 's';
    new_line[2] = '\0';
    new_line[3] = '\0';
    free(line);
    return new_line;
}

char *his_last_same_command(char *line, his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = malloc(sizeof(char) * 10);

    if (new_line == NULL)
        return NULL;
    new_line[0] = 'l';
    new_line[1] = 's';
    new_line[2] = '\0';
    new_line[3] = '\0';
    free(line);
    return new_line;
}

char *his_id_command(char *line, his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = malloc(sizeof(char) * 10);
    
    if (new_line == NULL)
        return NULL;
    new_line[0] = 'l';
    new_line[1] = 's';
    new_line[2] = '\0';
    new_line[3] = '\0';
    free(line);
    return new_line;
}

char *his_last_word(char *line, his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = malloc(sizeof(char) * 10);
    
    if (new_line == NULL)
        return NULL;
    new_line[0] = 'l';
    new_line[1] = 's';
    new_line[2] = '\0';
    new_line[3] = '\0';
    free(line);
    return new_line;
}

char *his_last_arg(char *line, his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = malloc(sizeof(char) * 10);

    if (new_line == NULL)
        return NULL;
    new_line[0] = 'l';
    new_line[1] = 's';
    new_line[2] = '\0';
    new_line[3] = '\0';
    free(line);
    return new_line;
}
