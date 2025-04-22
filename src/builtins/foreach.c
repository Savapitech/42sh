/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** foreach
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "exec.h"
#include "redirects.h"
#include "u_str.h"
#include "u_mem.h"
#include "loop.h"
#include "local.h"

static
bool checking_error(ef_t *ef, char **args)
{
    if (my_array_len(args) < 3)
        return (WRITE_CONST(STDERR_FILENO, "foreach: Too few arguments.\n"),
        true);
    if (!check_local_var(args[1], args[0]))
        return true;
    return false;
}

static
usr_cmd_t *buffers_realloc(usr_cmd_t *usr)
{
    char **new_buffers = u_realloc(usr->local_var, sizeof
        *usr->local_var * usr->sz, sizeof
        *usr->local_var * (usr->cap << 1));

    if (new_buffers == NULL)
        return NULL;
    usr->local_var = new_buffers;
    usr->cap <<= 1;
    return usr;
}

static
usr_cmd_t *get_first_cmd(usr_cmd_t *usr, char prompt[], size_t *bf_len)
{
    printf("%s", prompt);
    usr->local_var[usr->sz] = NULL;
    getline(&(usr->local_var[usr->sz]), bf_len, stdin);
    *bf_len = u_strlen(usr->local_var[usr->sz]);
    usr->local_var[usr->sz][*bf_len - 1] = '\0';
    usr->sz += 1;
    return usr;
}

static
usr_cmd_t *increase_buffers(usr_cmd_t *usr, size_t *buffer_len)
{
    usr->local_var[usr->sz] = NULL;
    getline(&(usr->local_var[usr->sz]), buffer_len, stdin);
    *buffer_len = u_strlen(usr->local_var[usr->sz]);
    usr->local_var[usr->sz][*buffer_len - 1] = '\0';
    usr->sz++;
    return usr;
}

static
usr_cmd_t *get_usr_loop_cmd(usr_cmd_t *usr_cmd)
{
    char prompt[] = "foreach? ";
    size_t buffer_len;

    usr_cmd->local_var = malloc(sizeof(char *) * usr_cmd->cap);
    if (usr_cmd->local_var == NULL)
        return NULL;
    usr_cmd = get_first_cmd(usr_cmd, prompt, &buffer_len);
    while (strcmp("end", usr_cmd->local_var[usr_cmd->sz - 1])){
        printf("foreach? ");
        if (usr_cmd->sz >= usr_cmd->cap)
            usr_cmd = buffers_realloc(usr_cmd);
        if (usr_cmd == NULL)
            return NULL;
        increase_buffers(usr_cmd, &buffer_len);
    }
    free(usr_cmd->local_var[usr_cmd->sz]);
    usr_cmd->sz--;
    usr_cmd->local_var[usr_cmd->sz] = NULL;
    return usr_cmd;
}

static
int do_a_lap(ef_t *ef, char **args, char **save_cmds)
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
    char **save_cmds = arraydup(usr_cmds->local_var);

    if (save_cmds == NULL)
        exit(84);
    for (int i = 2; args[i]; i++){
        if (!change_local(ef->exec_ctx->local, args[1], args[i]))
            exit(84);
        status = do_a_lap(ef, usr_cmds->local_var, save_cmds);
        free_array(args);
        args = arraydup(save_cmds);
    }
    free_array(usr_cmds->local_var);
    return status;
}

static
void launch_loop(ef_t *ef, char **args)
{
    int status = RETURN_FAILURE;
    usr_cmd_t *usr_cmds = malloc(sizeof(usr_cmd_t));

    if (usr_cmds == NULL)
        exit(84);
    usr_cmds->cap = 2;
    usr_cmds->sz = 0;
    signal(SIGINT, exit_child);
    signal(EOF, exit_child);
    usr_cmds = get_usr_loop_cmd(usr_cmds);
    if (usr_cmds == NULL)
        exit(84);
    status = foreach_loop(ef, args, usr_cmds);
    free_array(usr_cmds->local_var);
    exit(status);
}

int builtins_foreach(ef_t *ef, char **args)
{
    int status = 0;
    pid_t pid;

    if (checking_error(ef, args))
        return RETURN_FAILURE;
    printf("caok\n");
    pid = fork();
    if (pid == 0)
        launch_loop(ef, args);
    else
        wait(&status);
    if (WIFEXITED(status))
        ef->history->last_exit_code =
            ef->history->last_exit_code ?: WEXITSTATUS(status);
    return status;
}
