/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "exec.h"
#include "u_str.h"

int builtins_echo(ef_t *ef, char **args)
{
    bool newline = true;

    if (args[1] == nullptr)
        return WRITE_CONST(ef->out_fd, "\n"), RETURN_SUCCESS;
    for (size_t i = 1; args[i] != nullptr; i++) {
        if (strcmp("-n", args[i]) == 0) {
            newline = false;
            continue;
        }
        dprintf(ef->out_fd, "%s", args[i]);
        if (args[i + 1] != nullptr)
            dprintf(ef->out_fd, " ");
        else
            dprintf(ef->out_fd, "%s", newline ? "\n" : "");
    }
    return RETURN_SUCCESS;
}
