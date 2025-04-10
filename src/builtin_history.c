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

static
char *cat_in_str(his_variable_t *his_variable, char *str, char *cpy)
{
    int i = 0;
    int j = 0;
    int x = his_variable->coord_variable;
    int len_str = strlen(str) - 1;//pour l \n
    int size_right = len_str - x - his_variable->size_variable;
    int size_left = (len_str - size_right) - his_variable->size_variable;  
    char *new_str = malloc(sizeof(char) * (size_right + size_left + strlen(cpy) + 2));//ajout de deux \0
    //pas besoin de midlle car cpy existe 

    printf("Size right: %d\nSize left: %d\nSize cpy: %d\nSize \\0: 2\n", size_right , size_left , strlen(cpy));
    if (new_str == NULL)
        return NULL;
    new_str[0] = 0;
    for (; i < size_left; i++){
        printf("left\n");
        new_str[i] = str[i];
    }
    //printf("first loop; %s\n", new_str);
    /*
    **boucle for pour concatener cpy dans str
    */
    for (; cpy[j] != 0; j++){
        
        new_str[i] = cpy[j]; 
        i++;
    }
    printf("J-0; %d\n", j);
    //j+= his_variable->size_variable;
    printf("J-1; %d\n", j);
    for (int k = 0; k < size_right; k++){
        printf("right\n");
        new_str[i] = str[k + size_left + his_variable->size_variable];
        i++;
        j++;
    }
    new_str[i] = '\0';
    new_str[i + 1] = '\0'; 
    printf("New str: %s\n", new_str);
    free(new_str);
    return NULL;
}

char *his_last_command(char *line, his_variable_t *his_variable, his_command_t *his_command)
{
    char *new_line = malloc(sizeof(char) * 4);
    //char *new_str = NULL;

    if (new_line == NULL)
        return NULL;
    new_line[0] = 'l';
    new_line[1] = 's';
    new_line[2] = '\0';
    new_line[3] = '\0';
    //cat_in_str(his_variable, line, new_line);
    //printf("Adresse; %p && str; %s\n", new_line, new_line);
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
