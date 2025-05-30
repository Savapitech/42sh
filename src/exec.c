/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "args.h"
#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "debug.h"
#include "env.h"
#include "exec.h"
#include "job.h"
#include "path.h"
#include "repl.h"
#include "u_mem.h"
#include "u_str.h"
#include "utils.h"

const builtins_funcs_t BUILTINS[] = {
    { "builtins", &builtins_builtins },
    { "cd", &builtins_cd },
    { "chdir", &builtins_cd },
    { "env", &builtins_env },
    { "printenv", &builtins_env },
    { "setenv", &builtins_setenv },
    { "export", &builtins_setenv },
    { "unsetenv", &builtins_unsetenv },
    { ":", &builtins_funny_double_dot },
    { "repeat", &builtins_repeat },
    { "exit", &builtins_exit },
    { "history", &builtins_history},
    { "alias", &builtins_alias},
    { "yes", &builtins_yes },
    { "foreach", &builtins_foreach },
    { "while", &builtins_while },
    { "set", &builtins_set },
    { "unset", &builtins_unset },
    { "where", &builtins_where },
    { "which", &builtins_which },
    { "expr", &builtins_expr },
    { "if", &builtins_if },
    { "break", &builtins_break },
    { "astprint", &builtins_astprint },
    { "termname", &builtins_termname },
    { "echo", &builtins_echo },
    { "fg", &builtins_fg },
    { "bg", &builtins_bg },
    { "jobs", &builtins_jobs }
};

const size_t BUILTINS_SZ = sizeof BUILTINS / sizeof *BUILTINS;

__attribute__((nonnull))
bool ensure_args_capacity(args_t *args)
{
    char **new_ptr;

    if (args->sz + 1 < args->cap)
        return true;
    new_ptr = (char **)u_realloc((void *)args->args, sizeof *args->args *
        args->sz, sizeof *args->args * args->cap << 1);
    if (!new_ptr)
        return false;
    args->cap <<= 1;
    args->args = new_ptr;
    return true;
}

static
char **parse_args(ef_t *ef, ast_t *node)
{
    args_t args = { .args = (char **)malloc(sizeof *args.args
        * DEFAULT_ARGS_CAP), .sz = 0, .cap = DEFAULT_ARGS_CAP };

    if (!args.args)
        return nullptr;
    for (size_t i = 0; i < node->vector.sz; i++) {
        if (ef->skip_sz > 0 && i >= ef->skip_i && i < ef->skip_i + ef->skip_sz)
            continue;
        if (!process_args(node, &args, &i, ef))
            return free((void *)args.args), NULL;
    }
    if (!ensure_args_capacity(&args))
        return nullptr;
    args.args[args.sz] = nullptr;
    return args.args;
}

static
int command_error(char *cmd, char **args, int error)
{
    struct stat st;

    if (access(cmd, F_OK) == -1) {
            write(STDERR_FILENO, args[0], u_strlen(args[0]));
            WRITE_CONST(STDERR_FILENO, ": Command not found.\n");
            return 84;
    }
    stat(cmd, &st);
    if (S_ISDIR(st.st_mode) || access(cmd, X_OK)) {
            write(STDERR_FILENO, args[0], u_strlen(args[0]));
            WRITE_CONST(STDERR_FILENO, ": Permission denied.\n");
            return 84;
    }
    if (error == ENOEXEC) {
        write(STDERR_FILENO, args[0], u_strlen(args[0]));
        WRITE_CONST(STDERR_FILENO, ": Exec format error."
            " Binary file not executable.\n");
        return 0;
    }
    return 84;
}

static
void set_fd(ef_t *ef)
{
    U_DEBUG("In fd [%d] out fd [%d]\n", ef->in_fd, ef->out_fd);
    if (ef->in_fd != STDIN_FILENO) {
        dup2(ef->in_fd, STDIN_FILENO);
        close(ef->in_fd);
    }
    if (ef->out_fd != STDOUT_FILENO) {
        dup2(ef->out_fd, STDOUT_FILENO);
        close(ef->out_fd);
    }
}

static
int wait_child(ef_t *ef, int status, pid_t pid)
{
    bool untraced = (!(ef->flags & F_PIPE) || ef->p_i == ef->p_sz - 1);

    if (!init_child_job(ef->exec_ctx, pid, ef))
        return RETURN_FAILURE;
    if (ef->bg) {
        printf("[%lu] %d\n", ef->exec_ctx->jobs.sz, pid);
        return RETURN_SUCCESS;
    }
    waitpid(pid, &status, untraced ? WUNTRACED : WNOHANG);
    if (WIFSTOPPED(status)) {
        ef->exec_ctx->jobs.jobs[ef->exec_ctx->jobs.sz - 1].running = false;
        ef->exec_ctx->jobs.jobs[ef->exec_ctx->jobs.sz - 1].foreground = false;
        printf("\nSuspended\n");
    }
    return status;
}

static
int launch_bin(char *full_bin_path, char **args, ef_t *ef)
{
    int status = 0;
    pid_t pid = fork();

    if (pid == 0) {
        set_fd(ef);
        restore_term_flags(ef->exec_ctx);
        init_child_job(ef->exec_ctx, pid, ef);
        execve(full_bin_path, args, ef->env->env);
        status = command_error(full_bin_path, args, errno);
        exit(RETURN_FAILURE);
    }
    return wait_child(ef, status, pid);
}

static
void status_handler(int status)
{
    if (!WIFEXITED(status) && WIFSIGNALED(status) && WTERMSIG(status)) {
        if (WTERMSIG(status) == SIGSEGV)
            WRITE_CONST(STDERR_FILENO, "Segmentation fault");
        if (WTERMSIG(status) == SIGTRAP)
            WRITE_CONST(STDERR_FILENO, "Trace/BPT trap");
        if (WTERMSIG(status) == SIGFPE)
            WRITE_CONST(STDERR_FILENO, "Floating exception");
        if (WCOREDUMP(status) && (WTERMSIG(status) == SIGSEGV ||
            WTERMSIG(status) == SIGFPE || WTERMSIG(status) == SIGTRAP))
            WRITE_CONST(STDERR_FILENO, " (core dumped)");
        if (WTERMSIG(status) == SIGSEGV || WTERMSIG(status) == SIGFPE ||
            WTERMSIG(status) == SIGTRAP)
            WRITE_CONST(STDERR_FILENO, "\n");
    }
}

static
bool builtins_launcher(ef_t *ef, char **args)
{
    int bin_l = u_strlen(args[0]);

    U_DEBUG("In fd [%d] out fd [%d]\n", ef->in_fd, ef->out_fd);
    for (size_t i = 0; i < BUILTINS_SZ; i++) {
        if (u_strlen(BUILTINS[i].name) != bin_l)
            continue;
        if (u_strcmp(BUILTINS[i].name, args[0]) == 0) {
            restore_term_flags(ef->exec_ctx);
            ef->exec_ctx->history->last_exit_code =
                BUILTINS[i].ptr(ef, args);
            return true;
        }
    }
    return false;
}

int exec_the_args(ef_t *ef, char **args)
{
    char *full_bin_path;
    int status;

    if (builtins_launcher(ef, args))
        return RETURN_SUCCESS;
    full_bin_path = parse_full_bin_path(ef->env, args[0]);
    if (full_bin_path == NULL)
        return (free((void *)args), RETURN_FAILURE);
    U_DEBUG("Found bin [%s]\n", full_bin_path);
    status = launch_bin(full_bin_path, args, ef);
    status_handler(status);
    U_DEBUG("Exit code [%d]\n", ef->history->last_exit_code);
    free(full_bin_path);
    return status;
}

int execute(ef_t *ef)
{
    char **args;
    int status;

    args = parse_args(ef, ef->act_node);
    if (!args)
        return RETURN_FAILURE;
    status = exec_the_args(ef, args);
    if (WIFEXITED(status))
        ef->exec_ctx->history->last_exit_code =
            ef->exec_ctx->history->last_exit_code ?: WEXITSTATUS(status);
    free_args(args);
    init_shell_repl(ef->exec_ctx);
    if (ef->exec_ctx->isatty &&
        tcsetpgrp(ef->exec_ctx->read_fd, ef->exec_ctx->jobs.jobs[0].pgid) < 0)
        return RETURN_FAILURE;
    return ef->exec_ctx->history->last_exit_code
        != 0 ? RETURN_FAILURE : RETURN_SUCCESS;
}
