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

#include "builtins_handler.h"
#include "common.h"
#include "debug.h"
#include "env.h"
#include "history.h"
#include "local.h"
#include "readline.h"
#include "repl.h"
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
    if (buffer == NULL)
        return;
    if (*buffer == '|')
        WRITE_CONST(STDERR_FILENO, "Invalid null command.\n");
    if (*buffer == '>' || *buffer == '<')
        WRITE_CONST(STDERR_FILENO, "Missing name for redirect.\n");
}

static
void write_prompt(int is_a_tty, exec_ctx_t *exec_ctx)
{
    if (is_a_tty)
        print_shell_prompt(exec_ctx);
}

static
bool change_shell_command(buff_t *buff, exec_ctx_t *exec_ctx)
{
    char *tmp_buff = nullptr;
    size_t buffer_len;

    buff->sz = 0;
    if (!readline(exec_ctx, buff))
        return false;
    if (!buff->sz)
        return false;
    tmp_buff = buff->str;
    buffer_len = update_command(&tmp_buff, &buff->sz, exec_ctx);
    if (buffer_len == 0)
        return true;
    if (buffer_len < 1 || !u_str_is_alnum(tmp_buff))
        return check_basic_error(tmp_buff), true;
    U_DEBUG("Buffer [%lu] [%s]\n", buffer_len, tmp_buff);
    if (visitor(tmp_buff, exec_ctx) == RETURN_FAILURE
        && !exec_ctx->history->last_exit_code)
        exec_ctx->history->last_exit_code = RETURN_FAILURE;
    return true;
}

static
int shell_loop(int is_a_tty, exec_ctx_t *exec_ctx)
{
    buff_t buff = { .str = nullptr, 0, .cap = BUFF_INIT_SZ };

    init_shell_repl(exec_ctx);
    while (true) {
        write_prompt(is_a_tty, exec_ctx);
        if (!change_shell_command(&buff, exec_ctx))
            return exec_ctx->history->last_exit_code;
    }
    return free(buff.str), exec_ctx->history->last_exit_code;
}

static
his_command_t *init_cmd_history(void)
{
    his_command_t *cmd_history = malloc(sizeof(his_command_t) * 100);

    if (cmd_history == NULL)
        return nullptr;
    for (int i = 0; i != 100; i++){
        cmd_history[i].arg = nullptr;
        cmd_history[i].command = nullptr;
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

int shell(char **env_ptr)
{
    alias_t alias = init_alias();
    env_t env = parse_env(env_ptr);
    history_t history = { .cmd_history = nullptr, .last_exit_code = 0,
        .last_chdir = nullptr};
    his_command_t *cmd_history = init_cmd_history();
    local_t local = create_local();
    exec_ctx_t exec_ctx = {.env = &env, .local = &local,
        .history = &history, .history_command = cmd_history, .alias = &alias};
    int shell_result;

    if (error_in_init(&exec_ctx))
        return RETURN_FAILURE;
    U_DEBUG_CALL(debug_env_entries, &env);
    shell_result = shell_loop(isatty(STDIN_FILENO), &exec_ctx);
    if (isatty(STDIN_FILENO)) {
        WRITE_CONST(STDOUT_FILENO, "exit\n");
        restore_term_flags(&exec_ctx);
    }
    return free_everything(&exec_ctx), shell_result;
}
