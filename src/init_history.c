/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** init_history
*/

#include <fcntl.h>
#include <unistd.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "history.h"
#include "ctype.h"

static long long file_size(char *file)
{
    int fd = 0;
    char buffer[4096];
    long long size = 0;
    int somme = 0;

    fd = open(file, O_RDONLY);
    if (fd == -1)
    return -1;
    for (somme = 1; somme > 0; size += somme)
    somme = read(fd, buffer, 4095);
    if (somme == -1){
        close(fd);
        return -1;
    }
    close(fd);
    return size;
}

char *my_catchar(char *file)
{
    int fd = open(file, O_RDONLY);
    long long int size = file_size(file);
    char *str = malloc(sizeof(char) * (size + 1));
    int err = 0;

    if (str == NULL || fd == -1)
    return NULL;
    err = read(fd, str, size);
    if (err == -1)
    return NULL;
    close(fd);
    str[size] = '\0';
    return str;
}

static int check_delim(char *str, int x, char *delim, int before)
{
    for (int i = 0; delim[i] != '\0'; i++) {
        if (str[x] == delim[i] && before != 1)
            return 1;
    }
    return 0;
}

static int get_len(char *str, char *delim)
{
    int before = 0;
    int len = 1;

    if (!str)
        return -1;
    for (int x = 0; str[x] != '\0'; x++){
        if (check_delim(str, x, delim, before) == 1) {
            len++;
            before = 1;
        } else
            before = 0;
    }
    return len;
}

char **word_array_tok(char *str, char *delim)
{
    char *tmp;
    char *strToken;
    char **tab = NULL;
    int x = 0;

    if (!str || !delim)
        return tab;
    tmp = strdup(str);
    tab = malloc(sizeof(char *) * (get_len(tmp, delim) + 1));
    if (tab != NULL) {
    strToken = strtok(tmp, delim);
    while (strToken != NULL) {
        tab[x] = strdup(strToken);
        x++;
        strToken = strtok(NULL, delim);
    }
    tab[x] = NULL;
    }
    free(tmp);
    return tab;
}

static char *get_arg(char *line, int x, int end_cmd)
{
    char *tmp = malloc(sizeof(char) * (x - end_cmd) + 1);

    if (tmp != NULL) {
        tmp = strncpy(tmp, &line[end_cmd], x - end_cmd);
        tmp[x - end_cmd] = '\0';
    }
    return tmp;
}

his_command_t set_cmd(char *line, his_command_t cmd_struct)
{
    int x = 0;
    int end_cmd;

    while (line[x] != '\0' && !isblank(line[x]))
        x++;
    cmd_struct.command = malloc(sizeof(char) * x + 1);
    if (cmd_struct.command != NULL) {
        cmd_struct.command = strncpy(cmd_struct.command, line, x);
        cmd_struct.command[x] = '\0';
    }
    end_cmd = x + 1;
    while (line[x] != '\0')
        x++;
    if (x <= end_cmd)
        return cmd_struct;
    cmd_struct.arg = get_arg(line, x, end_cmd);
    return cmd_struct;
}

his_command_t *fill_cmd_history(his_command_t *cmd_history)
{
    char *buffer = my_catchar(".historique");
    char **saved;
    int i = 0;

    saved = word_array_tok(buffer, "\n");
    if (!saved)
        return cmd_history;
    for (; saved[i] != NULL || i >= 100; i++) {
        cmd_history[i] = set_cmd(saved[i], cmd_history[i]);
    }
    cmd_history->sz = i;
    free(buffer);
    for (int x = 0; saved[x]; x++)
        free(saved[x]);
    free(saved);
    return cmd_history;
}

int save_cmd_history(his_command_t *cmd_history)
{
    int fd = open(".historique", O_CREAT | O_TRUNC | O_WRONLY, 0644);

    if (fd == -1){
        return 84;
    }
    printf("passed here\n");
    for (int i = 0; i < cmd_history->sz; i++){
        if (cmd_history[i].command != NULL)
            write(fd, cmd_history[i].command, strlen(cmd_history[i].command));
        if (cmd_history[i].arg != NULL) {
            write(fd, " ", 1);
            write(fd, cmd_history[i].arg, strlen(cmd_history[i].arg));
        }
        free(cmd_history[i].command);
        free(cmd_history[i].arg);
        write(fd, "\n", 1);
    }
    close(fd);
    return 0;
}
