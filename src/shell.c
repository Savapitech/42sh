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
#include "history.h"
#include "exec.h"

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
void write_prompt(int is_a_tty)
{
    if (is_a_tty)
        WRITE_CONST(STDOUT_FILENO, SHELL_PROMPT);
}

/*
** Noeud de fonction
** Pour changer la commande
** passer en parametre
** si besoin
** historique, alias ...
*/
static
int shell_loop(int is_a_tty, builtin_handler_t *builtin_handler)
{
    char *buffer = NULL;
    size_t buffer_sz = 0;
    size_t buffer_len = 0;

    while (true) {
        write_prompt(is_a_tty);
        if (getline(&buffer, &buffer_sz, stdin) == -1)
            break;
        buffer_len = update_command(&buffer, &buffer_sz, builtin_handler);
        if (buffer_len < 1 || !u_str_is_alnum(buffer)) {
            check_basic_error(buffer);
            continue;
        }
        /*SAVE COMMAND pour evité le cas !4 !3*/
        U_DEBUG("Buffer [%lu] [%s]\n", buffer_len, buffer);
        visitor(buffer, builtin_handler);
        free(buffer);
    }
    free(builtin_handler->history_command);
    return (free(buffer), builtin_handler->history->last_exit_code);
}

his_command_t *init_cmd_history(void)
{
    his_command_t *cmd_history = malloc(sizeof(his_command_t) * 100);

    if (cmd_history == NULL)
        return NULL;
    for (int i = 0; i != 100; i++){
        cmd_history[i].arg = NULL;
        cmd_history[i].command = NULL;
        cmd_history[i].id = i;
    }
        
    cmd_history[0].command;
    return cmd_history;
}

/*
** verifier le retour du malloc et passer
** l initalisation de builtin handler dans
** une fonction pour l env l' history et les futurs builtins
*/
int shell(char **env_ptr)
{
    env_t env = parse_env(env_ptr);
    history_t history = { .cmd_history = NULL, 0, .last_chdir = NULL};
    his_command_t *cmd_history = init_cmd_history();
    builtin_handler_t builtin_handler = {.env = &env,
        .history = &history, .history_command = cmd_history};
    int shell_result;

    if (!cmd_history || !env.env){
        if (cmd_history)
            free(cmd_history);
        if (env.env)
            free(env.env);
        return RETURN_FAILURE;
    }
    U_DEBUG_CALL(debug_env_entries, &env);
    signal(SIGINT, ignore_sigint);
    shell_result = shell_loop(isatty(STDIN_FILENO), &builtin_handler);
    free_env(&env);
    return shell_result;
}
