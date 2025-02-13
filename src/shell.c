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
int shell_loop(env_t *env, int is_a_tty)
{
    char *buffer = NULL;
    size_t buffer_sz;
    size_t buffer_len;

    while (true) {
        if (is_a_tty)
            WRITE_CONST(STDOUT_FILENO, SHELL_PROMPT);
        if (getline(&buffer, &buffer_sz, stdin) == -1)
            break;
        buffer_len = u_strlen(buffer);
        if (buffer_len < 2 || !u_str_is_alnum(buffer))
            continue;
        if (buffer[buffer_len - 1] == '\n')
            buffer[buffer_len - 1] = '\0';
        U_DEBUG("Buffer [%lu] [%s]\n", buffer_len, buffer);
        execute(buffer, env);
    }
    if (is_a_tty)
        WRITE_CONST(STDOUT_FILENO, "exit\n");
    return (free(buffer), RETURN_SUCCESS);
}

int shell(char **env_ptr)
{
    env_t env = parse_env(env_ptr);
    int shell_result;

    if (!env.env)
        return RETURN_FAILURE;
    U_DEBUG_CALL(debug_env_entries, &env);
    shell_result = shell_loop(&env, isatty(STDIN_FILENO));
    free_env(&env);
    return shell_result;
}
