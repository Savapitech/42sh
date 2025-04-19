/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** repeat
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
bool checking_error(ef_t *ef, char **args, long *nb_loop)
{
    char *end;

    if (my_array_len(args) < 3)
        return (WRITE_CONST(STDERR_FILENO, "repeat: Too few arguments.\n"),
        true);
    *nb_loop = strtol(args[1], &end, 10);
    if (end[0] != '\0'){
        return (WRITE_CONST(STDERR_FILENO, "repeat: Badly formed number.\n"),
        true);
    }
    return false;
}

int builtins_repeat(ef_t *ef, char **args)
{
    long nb_loop = 0;
    int status = 0;

    if (checking_error(ef, args, &nb_loop))
        return RETURN_FAILURE;
    for (int i = 0; i < nb_loop; i++)
        status = exec_the_args(ef, &args[2]);
    return status;
}
