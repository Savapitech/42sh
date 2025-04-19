/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** yes
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "u_str.h"

static
int len_buffer(char **args)
{
    int len_buffer = 0;

    for (int i = 0; args[i]; i++)
        len_buffer += strlen(args[i]) + 1;
    return len_buffer;
}

static
char *define_prompt(char **args)
{
    char *buffer = NULL;

    if (args[1] == NULL)
        return u_strdup("y\n");
    buffer = malloc(sizeof(char) * (len_buffer(args) + 1));
    if (buffer == NULL)
        return NULL;
    strcpy(buffer, args[1]);
    for (int i = 2; args[i]; i++){
        strcat(buffer, args[i]);
        if (args[i + 1] != NULL)
            buffer[strlen(buffer) - 1] = ' ';
    }
    buffer[strlen(buffer) - 1] = '\n';
    return buffer;
}

int builtins_yes(ef_t *ef, char **args)
{
    char *buffer = define_prompt(args);
    pid_t pid;

    if (buffer == NULL)
        return RETURN_FAILURE;
    pid = fork();
    if (pid == 0){
        while (true){
            signal(SIGINT, exit_child);
            write(ef->out_fd, buffer, strlen(buffer));
        }
        exit(RETURN_SUCCESS);
    } else
        wait(NULL);
    free(buffer);
    return RETURN_SUCCESS;
}
