/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "common.h"
#include "debug.h"

int builtins_eval(ef_t *ef, char **args)
{
    char *clean_buff;
    size_t args_len = 0;

    if (args[1] == NULL)
        return RETURN_SUCCESS;
    clean_buff += strlen(args[0]);
    U_DEBUG("Eval buff len [%s]\n", ef->buffer);
    for (size_t i = 0; args[i] != NULL; i++)
        args_len += strlen(args[i]);
    clean_buff = malloc(sizeof(char) * args_len);
    if (clean_buff == NULL)
        return RETURN_FAILURE;
    visitor(clean_buff, ef->env, ef->history);
    return RETURN_SUCCESS;
}
