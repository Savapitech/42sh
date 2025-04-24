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
#include "history.h"
#include "shell.h"
#include "u_str.h"
#include "local.h"
#include "loop.h"

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
    if (buffer == NULL)
        return;
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

static
bool change_shell_command(char **buffer, exec_ctx_t *exec_ctx,
    size_t buffer_sz)
{
    size_t buffer_len = 0;
    char *tmp_buff = NULL;

    if (getline(buffer, &buffer_sz, stdin) == -1)
        return true;
    tmp_buff = (*buffer);
    buffer_len = update_command(&tmp_buff, &buffer_sz, exec_ctx);
    if (buffer_len == 0)
        return false;
    if (buffer_len < 1 || !u_str_is_alnum(tmp_buff)) {
        check_basic_error(tmp_buff);
        free(tmp_buff);
        return false;
    }
    U_DEBUG("Buffer [%lu] [%s]\n", buffer_len, buffer);
    visitor(tmp_buff, exec_ctx);
    free(tmp_buff);
    return false;
}

static
int shell_loop(int is_a_tty, exec_ctx_t *exec_ctx)
{
    char *buffer = NULL;
    size_t buffer_sz = 0;

    while (true) {
        write_prompt(is_a_tty);
        if (change_shell_command(&buffer, exec_ctx, buffer_sz) == true)
            return exec_ctx->history->last_exit_code;
    }
    free(exec_ctx->history_command);
    return (free(buffer), exec_ctx->history->last_exit_code);
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
    cmd_history->sz = 0;
    return cmd_history;
}

static
bool error_in_init(exec_ctx_t *exec_ctx)
{
    if (!exec_ctx->history_command || !exec_ctx->env->env
        || !exec_ctx->alias->alias_array ||
        !exec_ctx->alias->alias_to_replace) {
        free(exec_ctx->alias->alias_array);
        free(exec_ctx->alias->alias_to_replace);
        free(exec_ctx->history_command);
        free(exec_ctx->env->env);
        return true;
    }
    return false;
}

alias_t init_alias(void)
{
    alias_t alias;

    alias.size = 1;
    alias.alias_array = malloc(sizeof(char *) * alias.size);
    alias.alias_to_replace = malloc(sizeof(char *) * alias.size);
    if (!alias.alias_array || !alias.alias_to_replace)
        return alias;
    alias.alias_array[0] = NULL;
    alias.alias_to_replace[0] = NULL;
    alias.size = 0;
    return alias;
}

int shell(char **env_ptr)
{
    alias_t alias = init_alias();
    env_t env = parse_env(env_ptr);
    history_t history = { .cmd_history = NULL, 0, .last_chdir = NULL};
    his_command_t *cmd_history = init_cmd_history();
    local_t local = create_local();
    exec_ctx_t exec_ctx = {.env = &env, .local = &local,
        .history = &history, .history_command = cmd_history, .alias = &alias};
    int shell_result;

    if (error_in_init(&exec_ctx) == true){
        return RETURN_FAILURE;
    }
    U_DEBUG_CALL(debug_env_entries, &env);
    signal(SIGINT, ignore_sigint);
    shell_result = shell_loop(isatty(STDIN_FILENO), &exec_ctx);
    if (isatty(STDIN_FILENO))
        WRITE_CONST(STDOUT_FILENO, "exit\n");
    free_env(exec_ctx.env);
    free_alias(exec_ctx.alias);
    return shell_result;
}
