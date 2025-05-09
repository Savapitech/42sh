/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <unistd.h>

#include "exec.h"
#include "u_str.h"

int builtins_echo(ef_t *ef, char **args)
{
    if (args[1] == nullptr)
        return WRITE_CONST(ef->out_fd, "\n"), RETURN_SUCCESS;
    for (size_t i = 1; args[i] != nullptr; i++) {
        dprintf(ef->out_fd, "%s", args[i]);
        if (args[i + 1] != nullptr)
            dprintf(ef->out_fd, " ");
        else
            dprintf(ef->out_fd, "\n");
    }
    return RETURN_SUCCESS;
}
