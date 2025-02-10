/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "debug.h"
#include "env.h"
#include "shell.h"
#include "u_str.h"

static
int shell_loop(void)
{
    int is_a_tty = isatty(STDIN_FILENO);
    char *buffer = NULL;
    size_t buffer_sz;

    signal(SIGINT, SIG_IGN);
    while (true) {
        if (is_a_tty)
            WRITE_CONST(STDOUT_FILENO, SHELL_PROMPT);
        if (getline(&buffer, &buffer_sz, stdin) == -1)
            break;
        U_DEBUG("Buffer [%.*s]\n", u_strlen(buffer) - 1, buffer);
        if (!u_str_is_alnum(buffer))
            continue;
    }
    free(buffer);
    return RETURN_SUCCESS;
}

int shell(char **env)
{
    buff_t env_values = { 0 };
    env_entry_t *env_entries = NULL;
    size_t env_size = count_env_entries(env);

    env_values.str = malloc(ENV_BUFF_CAP);
    if (env_values.str == NULL)
        return RETURN_FAILURE;
    env_entries = malloc(sizeof *env_entries * env_size);
    parse_env(env, &env_values, env_entries);
    U_DEBUG_CALL(debug_env_entries, env_entries, env_size);
    return shell_loop();
}
