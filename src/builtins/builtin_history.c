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

#include <string.h>
#include "utils.h"
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>

static char *concat_cmd_arg(char *dest, char *src)
{
    int l;
    int i;
    char *r_value = NULL;

    if (!src) {
        r_value = u_strdup(dest);
        return r_value;
    } else {
        l = strlen(dest);
        i = strlen(src);
        r_value = malloc(sizeof(char)* (i + l + 2));
        if (r_value != NULL) {
            strcpy(r_value, dest);
            r_value[l] = ' ';
            r_value[l +1] = '\0';
            strcat(r_value, src);
        }
    }
    return r_value;
}

char *his_last_command(char *line,
    his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = NULL;
    char *new_str = NULL;

    if (his_command->sz == 0){
        printf("%d: Event not found\n", his_command->sz);
        return NULL;
    }
    new_line = concat_cmd_arg(his_command[his_command->sz - 1].command,
        his_command[his_command->sz - 1].arg);
    new_str = cat_in_str(his_variable, line, new_line);
    printf("%s\n", new_line);
    free(new_line);
    free(line);
    return new_str;
}

char *his_last_same_command(char *line,
    his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = &line[his_variable->coord_variable + 1];
    char *new_str = NULL;

    for (int i = his_command->sz - 1; i >= 0; i--) {
        if (his_command[i].command == NULL) {
            printf("%s: Event not found\n", new_line);
            return NULL;
        }
        if (strncmp(his_command[i].command, new_line, strlen(new_line)) == 0) {
            new_line = concat_cmd_arg(his_command[i].command,
                his_command[i].arg);
            new_str = cat_in_str(his_variable, line, new_line);
            free(line);
            return new_str;
        }
    }
    printf("%s: Event not found\n", new_line);
    free(line);
    return NULL;
}

char *his_id_command(char *line,
    his_variable_t *his_variable, his_command_t *his_command)
{
    int id = atoi(&line[his_variable->coord_variable + 1]);
    char *new_line;
    char *new_str = NULL;

    if (his_command[id].command == NULL){
        printf("%d: Event not found\n", id);
        return new_str;
    }
    new_line = concat_cmd_arg(his_command[id].command, his_command[id].arg);
    new_str = cat_in_str(his_variable, line, new_line);
    printf("%s\n", new_line);
    free(new_line);
    free(line);
    return new_str;
}

static char *get_last_word(char *str)
{
    char *last_word = NULL;
    int last_space = 0;
    int x = 0;

    if (!str)
        return NULL;
    while (str[x] != '\0') {
        if (isblank(str[x]))
            last_space = x + 1;
        x++;
    }
    last_word = malloc(sizeof(char) * (x - last_space) + 1);
    if (last_word != NULL) {
        last_word = strncpy(last_word, &str[last_space], x - last_space);
        last_word[x - last_space] = '\0';
    }
    return last_word;
}

char *his_last_word(char *line,
    his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = NULL;
    char *new_str = NULL;

    if (his_command[his_command->sz - 1].arg == NULL){
        new_line = get_last_word(his_command[his_command->sz - 1].command);
    } else
        new_line = get_last_word(his_command[his_command->sz - 1].arg);
    if (!new_line)
        return NULL;
    new_str = cat_in_str(his_variable, line, new_line);
    printf("%s\n", new_line);
    free(new_line);
    free(line);
    return new_str;
}

char *his_last_arg(char *line,
    his_variable_t *his_variable, his_command_t *his_command)
{
    int id = his_command->sz - 1;
    char *new_line = NULL;
    char *new_str = NULL;

    if (!his_command[id].arg)
        new_line = u_strdup(" ");
    else
        new_line = u_strdup(his_command[id].arg);
    new_str = cat_in_str(his_variable, line, new_line);
    printf("%s\n", new_line);
    free(new_line);
    free(line);
    return new_str;
}
