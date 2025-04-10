/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** update_command
*/
#include "history.h"
#include "builtins_handler.h"
#include "u_str.h"

size_t update_command(char **buffer,
    size_t *buffer_sz, builtin_handler_t *builtin_handler)
{
    size_t buffer_len = 0;

    buffer_len = u_strlen(*buffer);
    (*buffer)[buffer_len - 1] = '\0';
    /*parse_alias*/
    parse_history(buffer, &buffer_len,
        buffer_sz, &builtin_handler->history_command);
    return buffer_len;
}
