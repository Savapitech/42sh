/*
** EPITECH PROJECT, 2025
** history_42sh
** File description:
** his for history
*/
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "env.h"
#include "exec.h"
#include "u_mem.h"
#include "u_str.h"
#include "history.h"

const parsing_history_t tab_fnct[] = {
    {"!!", &his_last_command}, //last command
    {"!$", &his_last_word}, //last word command
    {"!*", &his_last_arg}, //last argument commmand
    {"![command]", &his_last_same_command}, //derniere commande + arg sur la derniere meme command dans l historique dans le cas ou il n y a qu un charactère il prend le dernier qui commence par la meme chaine
    {"![number]", &his_id_command}, //id command
    {NULL, NULL},
};

static
int cmd_history_is_in(char *line)
{
    for (int i = 0; line[i] != 0; i++)
        if (line[i] == CHAR_HIST &&
            (line[i + 1] != ' ' && line[i + 1] != '\t'))
            return 0;
    return 1;
}

/*
** Pour savoir si c est
** des builtins a deux
** charactères
*/
static
int is_two_char_cmd(char *line, int coord_x)
{
    if (line[coord_x] != CHAR_HIST)
        return -1;
    coord_x++;
    switch (line[coord_x]){
        case '!':
            return 0;
        case '$':
            return 1;
        case '*':
            return 2;
    }
    return -1;
}

static
int which_his_cmd(his_variable_t *his_variable, char const *line)
{
    for (int i = 0; line[i] != '\0'; i++){
        his_variable->type = is_two_char_cmd(line, i);
        if (his_variable->type != -1){
            his_variable->coord_variable = i;
            his_variable->size_variable = 2;
            return 0;
        }
    }
    return 0;
}

static
char *replace_history(char *line)
{
    his_variable_t his_variable = {0, 0, -1};

    which_his_cmd(&his_variable, line);
    while (his_variable.type != -1){
        line = tab_fnct[his_variable.type].funct(line, &his_variable, NULL);
        if (line == NULL)
            return NULL;
        which_his_cmd(&his_variable, line);
    }
    return line;
}

int parse_history(char **pointer_line,
    size_t *buffer_len, size_t *buffer_sz, his_command_t **cmd_history)//faut la passer au autre fonction
{
    char *line = *pointer_line;

    *buffer_sz = 0;
    if (cmd_history_is_in(line) == 0){
        line = replace_history(line);
        if (line == NULL)
            return 84;
        *buffer_len = u_strlen(line) + 1;
        *pointer_line = line;
        return 0;
    }
    return 0;
}
