/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtins.h"
#include "common.h"
#include "debug.h"
#include "env.h"
#include "exec.h"
#include "u_mem.h"
#include "u_str.h"

static
const builtins_t BUILTINS[] = {
    { "cd", &builtins_cd },
    { "env", &builtins_env },
    { "setenv", &builtins_setenv },
    { "unsetenv", &builtins_unsetenv },
    { "exit", &builtins_exit }
};

static
const size_t BUILTINS_SZ = sizeof BUILTINS / sizeof *BUILTINS;

static
char *build_full_path(const char *token, const char *binary)
{
    size_t len_token = u_strlen(token);
    size_t len_bin = u_strlen(binary);
    char *full_path = malloc(len_token + len_bin + 2);

    if (!full_path)
        return NULL;
    u_strcpy(full_path, token);
    full_path[len_token] = '/';
    u_strcpy(full_path + len_token + 1, binary);
    full_path[len_token + len_bin + 1] = '\0';
    return full_path;
}

static
char *find_binary(const char *path_env, const char *binary)
{
    static char *saveptr = NULL;
    static char *path = NULL;
    char *token;
    char *full_path;

    if (path_env) {
        path = u_strdup(path_env);
        if (!path)
            return NULL;
        token = strtok_r(path, ":", &saveptr);
    } else
        token = strtok_r(NULL, ":", &saveptr);
    if (!token)
        return free(path), u_strdup(binary);
    full_path = build_full_path(token, binary);
    if (!full_path)
        return NULL;
    return access(full_path, X_OK) == 0 ? full_path : (free(full_path),
        find_binary(NULL, binary));
}

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
char **parse_args(char *buffer)
{
    size_t sz = 0;
    size_t cap = DEFAULT_ARGS_CAP;
    char **args = (char **)malloc(sizeof *args * cap);
    char *token;

    if (!args)
        return NULL;
    token = strtok(buffer, " \t");
    while (token != NULL) {
        ensure_args_capacity(&args, sz, &cap);
        args[sz] = token;
        U_DEBUG("Args [%lu] [%s]\n", sz, args[sz]);
        sz++;
        token = strtok(NULL, " \t");
    }
    ensure_args_capacity(&args, sz, &cap);
    args[sz] = NULL;
    return args;
}

static
int launch_bin(char *full_bin_path, char **args, env_t *env, char *buff)
{
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        if (execve(full_bin_path, args, env->env) < 0) {
            write(STDERR_FILENO, args[0], u_strlen(args[0]));
            WRITE_CONST(STDOUT_FILENO, ": Command not found.\n");
            free_env(env);
            free((void *)args);
            free(buff);
            exit(127);
        }
    }
    waitpid(pid, &status, 0);
    return status;
}

static
void signal_handler(int sig)
{
    switch (sig) {
        case SIGSEGV:
            WRITE_CONST(STDOUT_FILENO, "Segmentation fault\n");
            break;
        case SIGILL:
            WRITE_CONST(STDOUT_FILENO, "Illegal instruction\n");
            break;
        default:
            WRITE_CONST(STDOUT_FILENO, "Unknown signal\n");
    }
}

static
void status_handler(int status, history_t *history)
{
    if (WIFEXITED(status))
        history->last_exit_code = WEXITSTATUS(status);
    else if (WIFSIGNALED(status)) {
        history->last_exit_code = RETURN_FAILURE;
        signal_handler(WTERMSIG(status));
    }
    U_DEBUG("Exit code [%d]\n", history->last_exit_code);
}

int execute(char *buffer, env_t *env, history_t *history)
{
    char *path = NULL;
    char *full_bin_path;
    char **args = parse_args(buffer);
    int status;

    if (!args)
        return RETURN_FAILURE;
    for (size_t i = 0; i < BUILTINS_SZ; i++)
        if (u_strcmp(buffer, BUILTINS[i].name) == 0)
            return BUILTINS[i].ptr(env, args, buffer, history);
    path = get_env_value(env, "PATH");
    full_bin_path = find_binary(path, args[0]);
    if (full_bin_path == NULL)
        return (free((void *)args), RETURN_FAILURE);
    U_DEBUG("Found bin [%s]\n", full_bin_path);
    status = launch_bin(full_bin_path, args, env, buffer);
    status_handler(status, history);
    free(full_bin_path);
    free((void *)args);
    return 0;
}
