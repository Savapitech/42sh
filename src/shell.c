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

#include "ast.h"
#include "common.h"
#include "debug.h"
#include "env.h"
#include "shell.h"
#include "u_str.h"

__attribute__((unused))
static
void debug_env_entries(env_t *env)
{
    for (size_t i = 0; i < env->sz; i++) {
        if (env->env[i] == NULL)
            continue;
        U_DEBUG("Env entry [%lu] [%s]\n", i, env->env[i]);
    }
}

static
void check_basic_error(char const *buffer)
{
    if (*buffer == '|')
        WRITE_CONST(STDERR_FILENO, "Invalid null command.\n");
    if (*buffer == '>' || *buffer == '<')
        WRITE_CONST(STDERR_FILENO, "Missing name for redirect.\n");
}

static
void ignore_sigint(int sig __attribute__((unused)))
{
    WRITE_CONST(STDIN_FILENO, "\n");
    WRITE_CONST(STDOUT_FILENO, SHELL_PROMPT);
}

static
int shell_loop(env_t *env, int is_a_tty, history_t *history)
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
        if (buffer_len < 2 || !u_str_is_alnum(buffer)) {
            check_basic_error(buffer);
            continue;
        }
        buffer[buffer_len - 1] = '\0';
        U_DEBUG("Buffer [%lu] [%s]\n", buffer_len, buffer);
        visitor(buffer, env, history);
    }
    return (free(buffer), history->last_exit_code);
}

int shell(char **env_ptr)
{
    env_t env = parse_env(env_ptr);
    history_t history = { .cmd_history = NULL, 0, .last_chdir = NULL };
    int shell_result;

    if (!env.env)
        return RETURN_FAILURE;
    U_DEBUG_CALL(debug_env_entries, &env);
    signal(SIGINT, ignore_sigint);
    shell_result = shell_loop(&env, isatty(STDIN_FILENO), &history);
    free_env(&env);
    WRITE_CONST(STDOUT_FILENO, "exit\n");
    return shell_result;
}
