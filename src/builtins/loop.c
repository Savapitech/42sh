/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** foreach
*/

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "debug.h"
#include "exec.h"
#include "local.h"
#include "loop.h"
#include "u_str.h"

void exit_child(int sig[[gnu::unused]])
{
    _exit(sig);
}

static
bool checking_for_error(char **args)
{
    if (my_array_len(args) < 3)
        return (WRITE_CONST(STDERR_FILENO, "foreach: Too few arguments.\n"),
        true);
    if (check_local_var(args[1], args[0]))
        return true;
    return false;
}

static
bool checking_while_error(char **args)
{
    if (my_array_len(args) < 2)
        return (WRITE_CONST(STDERR_FILENO, "while: Too few arguments.\n"),
        true);
    if (my_array_len(args) > 2)
        return (WRITE_CONST(STDERR_FILENO, "while: Expression Syntax.\n"),
        true);
    return false;
}

static
int do_a_lap(ef_t *ef, char **args)
{
    int status = 0;

    for (int i = 0; args[i]; i++)
        status = visitor(args[i], ef->exec_ctx);
    return status;
}

static
int foreach_loop(ef_t *ef, char **args, usr_cmd_t *usr_cmds)
{
    int status = 0;
    char **save_cmds = arraydup(usr_cmds->cmds);

    if ((void *)save_cmds == nullptr)
        exit(84);
    for (int i = 2; args[i]; i++){
        if (!set_local(ef->exec_ctx->local, args[1], args[i]))
            exit(84);
        status = do_a_lap(ef, usr_cmds->cmds);
        free_array(usr_cmds->cmds);
        usr_cmds->cmds = arraydup(save_cmds);
    }
    free_array(save_cmds);
    return status;
}

static
int while_loop(ef_t *ef, usr_cmd_t *usr_cmds, char **args)
{
    int status = 0;
    char **save_cmds = arraydup(usr_cmds->cmds);
    int expr_result;

    if ((void *)save_cmds == nullptr)
        exit(84);
    expr_result = builtins_expr(ef, args);
    if (expr_result == -1)
        return RETURN_FAILURE;
    while (expr_result != 0) {
        U_DEBUG("While expr result [%d]\n", expr_result);
        status = do_a_lap(ef, usr_cmds->cmds);
        free_array(usr_cmds->cmds);
        usr_cmds->cmds = arraydup(save_cmds);
        expr_result = builtins_expr(ef, args);
        if (expr_result == -1)
            return RETURN_FAILURE;
    }
    free_array(save_cmds);
    return status;
}

static
int choose_loop(ef_t *ef, char **args, usr_cmd_t *usr_cmd, char const *prompt)
{
    if (strcmp(prompt, "foreach") == 0)
        return foreach_loop(ef, args, usr_cmd);
    return while_loop(ef, usr_cmd, args);
}

static
void launch_loop(ef_t *ef, char **args, char const *prompt)
{
    int status = RETURN_FAILURE;
    usr_cmd_t *usr_cmds = malloc(sizeof(usr_cmd_t));

    ef->exec_ctx->local->in_a_loop = true;
    if (usr_cmds == nullptr)
        exit(84);
    usr_cmds->cap = 2;
    usr_cmds->sz = 0;
    signal(SIGINT, exit_child);
    signal(EOF, exit_child);
    usr_cmds = get_usr_loop_cmd(ef->exec_ctx, usr_cmds, prompt);
    if (usr_cmds == nullptr)
        exit(84);
    status = choose_loop(ef, args, usr_cmds, prompt);
    free_array(usr_cmds->cmds);
    free(usr_cmds);
    exit(status);
}

int builtins_foreach(ef_t *ef, char **args)
{
    int status = 0;
    pid_t pid;

    if (checking_for_error(args))
        return RETURN_FAILURE;
    pid = fork();
    if (pid == 0)
        launch_loop(ef, args, "foreach");
    else
        wait(&status);
    if (WIFEXITED(status))
        ef->history->last_exit_code =
            ef->history->last_exit_code ?: WEXITSTATUS(status);
    return status;
}

int builtins_while(ef_t *ef, char **args)
{
    int status = 0;
    pid_t pid;

    if (checking_while_error(args))
        return RETURN_FAILURE;
    pid = fork();
    if (pid == 0)
        launch_loop(ef, args, "while");
    else
        wait(&status);
    if (WIFEXITED(status))
        ef->history->last_exit_code =
            ef->history->last_exit_code ?: WEXITSTATUS(status);
    return status;
}
