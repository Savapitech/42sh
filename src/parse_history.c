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
#include <string.h>

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

//const size_t TOKENS_LIST_SZ = sizeof TOKENS_LIST / sizeof *TOKENS_LIST;

int my_atoi(const char *str)
{
    int result = 0;
    int sign = 1;

    while ((*str > '9' && *str < '0') || *str == '-')
        str++;
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    for (; *str >= '0' && *str <= '9'; str++) {
        result = result * 10 + (*str - '0');
    }
    return result * sign;
}

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
bool is_a_token(char *str, int index_str)
{
    str+=index_str;
    for (size_t i = 0; i < 16; i++) {
        if (strncmp(str, TOKENS_LIST[i].str, 2) == 0){
            return true;
        }
    }
    return false;
}

static
char *strn_to_ndup(int start, int size, char *str)
{
    char *new_str = malloc(sizeof(char) * (size + 1));
    int count = 0;

    if (new_str == NULL)
        return NULL;
    new_str[size] = '\0';
    for (int i = start; i != start + size; i++){
        new_str[count] = str[i];
        count++;
    }
    return new_str;
}

static
int choose_id_or_last(his_variable_t *his_variable, int index_str, char *str)
{
    int mode = 0;
    const int cpy_index = index_str; 
    char *last_or_id = NULL;

    if (str[index_str] != CHAR_HIST && is_a_token(str, index_str + 1) == false)
        return -1;
    index_str++;
    for (;str[index_str] != 0; index_str++){
        if (is_a_token(str, index_str) == true || isblank(str[index_str]))
            break;
        if (!isdigit(str[index_str]))
            mode = 1;
    }
    his_variable->coord_variable = cpy_index;
    his_variable->size_variable = index_str - cpy_index;
    his_variable->str = strn_to_ndup(cpy_index, (index_str - cpy_index), str);
    if (his_variable->str == NULL)
        return 3;//La fonction last command dois verifier le NULL
    his_variable->id = my_atoi(his_variable->str + 1);
    return (mode == 1) ? 3 : 4;
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
        his_variable->type = choose_id_or_last(his_variable, i, line);
        if (his_variable->type != -1)
            return 0;
    }
    return 0;
}

static
char *replace_history(char *line)
{
    his_variable_t his_variable = {.coord_variable = 0, .id = 0, .size_variable = 0, .str = NULL, .type = -1};

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
        //printf("JE REMPLACE...\n......\n....\n..\n.\n");
        line = replace_history(line);
        if (line == NULL)
            return 84;
        *buffer_len = u_strlen(line) + 1;
        *pointer_line = line;
        return 0;
    }
    return 0;
}
