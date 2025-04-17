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

static char *get_arg(char *line, int x, int end_cmd)
{
    char *tmp = malloc(sizeof(char) * ((x - end_cmd) + 1));

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
    cmd_struct.command = malloc((sizeof(char) * x + 1));
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
