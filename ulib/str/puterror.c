/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "u_str.h"

void puterror(char const *prefix)
{
    char const *error_msg = strerror(errno);
    size_t len;

    if (error_msg == NULL)
        return;
    if (prefix != NULL && *prefix) {
        len = u_strlen(prefix);
        write(STDERR_FILENO, prefix, len);
        WRITE_CONST(STDERR_FILENO, ": ");
    }
    len = u_strlen(error_msg);
    write(STDERR_FILENO, error_msg, len);
    WRITE_CONST(STDERR_FILENO, ".\n");
}
