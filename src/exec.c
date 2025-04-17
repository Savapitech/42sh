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

#include "ast.h"
#include "builtins.h"
#include "common.h"
#include "debug.h"
#include "env.h"
#include "exec.h"
#include "path.h"
#include "u_mem.h"
#include "u_str.h"
#include "alias.h"

const builtins_funcs_t BUILTINS[] = {
    { "builtins", &builtins_builtins },
    { "cd", &builtins_cd },
    { "chdir", &builtins_cd },
    { "env", &builtins_env },
    { "printenv", &builtins_env },
    { "setenv", &builtins_setenv },
    { "unsetenv", &builtins_unsetenv },
    { ":", &builtins_funny_double_dot },
    { "exit", &builtins_exit },
    { "history", &builtins_history},
    { "alias", &builtins_alias}
};

const size_t BUILTINS_SZ = sizeof BUILTINS / sizeof *BUILTINS;

static __attribute__((nonnull))
bool ensure_args_capacity(char ***args, size_t const sz, size_t *cap)
{
    char **new_ptr;

    if (sz < *cap)
        return true;
    new_ptr = (char **)u_realloc((void *)*args, sizeof *args * sz,
        sizeof *args * *cap << 1);
    if (!new_ptr)
        return false;
    *cap <<= 1;
    *args = new_ptr;
    return true;
}

static
char **parse_args(ef_t *ef, ast_t *node, env_t *env)
{
    size_t sz = 1;
    size_t cap = DEFAULT_ARGS_CAP;
    char **args = (char **)malloc(sizeof *args * cap);

    if (!args)
        return NULL;
    node->tok.str[node->tok.sz] = '\0';
    args[0] = node->tok.str;
    for (size_t i = 0; i < node->vector.sz; i++) {
        if (ef->skip_sz > 0 && i >= ef->skip_i && i < ef->skip_i + ef->skip_sz)
            continue;
        ensure_args_capacity(&args, sz, &cap);
        args[sz] = handle_var_case(node, env, &i);
        if (args[sz] == NULL)
            return free(args), NULL;
        sz++;
    }
    ensure_args_capacity(&args, sz, &cap);
    args[sz] = NULL;
    return args;
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
int launch_bin(char *full_bin_path, char **args, ef_t *ef)
{
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        set_fd(ef);
        if (execve(full_bin_path, args, ef->env->env) < 0) {
            status = command_error(full_bin_path, args, errno);
            free_env(ef->env);
            free((void *)args);
            exit((free(ef->buffer), status));
        }
    }
    if (!(ef->flags & F_PIPE) || ef->p_i == ef->p_sz - 1)
        waitpid(pid, &status, 0);
    else
        waitpid(pid, &status, WNOHANG);
    if (WIFEXITED(status))
        ef->exec_ctx->history->last_exit_code =
        ef->exec_ctx->history->last_exit_code ?: WEXITSTATUS(status);
    return status;
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
            ef->exec_ctx->history->last_exit_code =
                BUILTINS[i].ptr(ef, args);
            return true;
        }
    }
    return false;
}

int execute(ef_t *ef)
{
    char *full_bin_path;
    char **args;
    int status;

    args = parse_args(ef, ef->act_node, ef->env);
    if (!args)
        return RETURN_FAILURE;
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
    free((void *)args);
    return ef->exec_ctx->history->last_exit_code
        != 0 ? RETURN_FAILURE : RETURN_SUCCESS;
}
