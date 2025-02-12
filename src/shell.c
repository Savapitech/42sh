/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "debug.h"
#include "env.h"
#include "exec.h"
#include "shell.h"
#include "u_str.h"

static
int shell_loop(env_t *env)
{
    int is_a_tty = isatty(STDIN_FILENO);
    char *buffer = NULL;
    size_t buffer_sz;

    while (true) {
        if (is_a_tty)
            WRITE_CONST(STDOUT_FILENO, SHELL_PROMPT);
        if (getline(&buffer, &buffer_sz, stdin) == -1)
            break;
        if (u_strlen(buffer) < 2)
            continue;
        if (!u_str_is_alnum(buffer))
            continue;
        U_DEBUG("Buffer [%.*s]\n", u_strlen(buffer) - 1, buffer);
        execute(buffer, env);
    }
    WRITE_CONST(STDOUT_FILENO, "exit\n");
    free(buffer);
    return RETURN_SUCCESS;
}

int shell(char **env_ptr)
{
    env_t env = parse_env(env_ptr);

    if (!env.env)
        return RETURN_FAILURE;
    U_DEBUG_CALL(debug_env_entries, &env);
    return shell_loop(&env);
}
