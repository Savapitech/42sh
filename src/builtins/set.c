/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** set
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "exec.h"
#include "u_str.h"

void print_local(ef_t *ef)
{
    for (size_t i = 0; i < ef->exec_ctx->local->sz; i++) {
        if (ef->exec_ctx->local->local_var[i] == NULL)
            continue;
        write(ef->out_fd, ef->exec_ctx->local->local_var[i],
            u_strlen(ef->exec_ctx->local->local_var[i]));
        WRITE_CONST(ef->out_fd, "\n");
    }
}

int special_case(ef_t *ef, char **args)
{
    int piv = 0;
    char *key = nullptr;
    char *val = nullptr;

    if (args[1] == NULL)
        return print_local(ef), RETURN_SUCCESS;
    for (; args[1][piv] && args[1][piv] != '='; piv++);
    key = strndup(args[1], piv);
    if (key == NULL)
        return RETURN_FAILURE;
    if (check_local_var(key, args[0]))
        return free(key), RETURN_FAILURE;
    if (args[1][piv] != '\0')
        val = &args[1][piv + 1];
    if (!set_local(ef->exec_ctx->local, key, val))
        return free(key), RETURN_FAILURE;
    free(key);
    return RETURN_SUCCESS;
}

static
bool handle_special_variables(ef_t *ef, char **args, char *var, int i)
{
    if (strcmp(var, "ignoreof") == 0) {
        ef->exec_ctx->ignoreof = true;
        return true;
    }
    if (args[i] == nullptr)
        return false;
    if (strcmp(var, "precmd") == 0) {
        ef->exec_ctx->precmd = strdup(args[i]);
        return true;
    }
    if (strcmp(var, "cwdcmd") == 0) {
        ef->exec_ctx->cwdcmd = strdup(args[i]);
        return true;
    }
    return false;
}

static
int handle_set(ef_t *ef, char **args, char *var)
{
    for (int i = 1; args[i]; i++){
        if (check_local_var(args[i], args[0]))
            return RETURN_FAILURE;
        var = args[i];
        i++;
        if (handle_special_variables(ef, args, var, i))
            return RETURN_SUCCESS;
        if (!args[i])
            return (set_local(ef->exec_ctx->local, var, nullptr)
            , RETURN_SUCCESS);
        if (strcmp(args[i], "=") != 0)
            return RETURN_FAILURE;
        i++;
        if (!set_local(ef->exec_ctx->local, var, args[i]))
            return RETURN_FAILURE;
    }
    return RETURN_SUCCESS;
}

int builtins_set(ef_t *ef, char **args)
{
    char *var = nullptr;

    if (args[1] != nullptr && handle_special_variables(ef, args, args[1], 2))
        return RETURN_SUCCESS;
    if (my_array_len(args) < 3)
        return special_case(ef, args);
    return handle_set(ef, args, var);
}
