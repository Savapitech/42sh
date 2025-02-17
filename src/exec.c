/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtins.h"
#include "common.h"
#include "debug.h"
#include "env.h"
#include "exec.h"
#include "u_mem.h"
#include "u_str.h"

const builtins_t BUILTINS[] = {
    { "builtins", &builtins_builtins },
    { "cd", &builtins_cd },
    { "chdir", &builtins_cd },
    { "env", &builtins_env },
    { "setenv", &builtins_setenv },
    { "unsetenv", &builtins_unsetenv },
    { "exit", &builtins_exit }
};

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
    token = strtok(buffer, " \t\v");
    while (token != NULL) {
        ensure_args_capacity(&args, sz, &cap);
        args[sz] = token;
        U_DEBUG("Args [%lu] [%s]\n", sz, args[sz]);
        sz++;
        token = strtok(NULL, " \t\v");
    }
    ensure_args_capacity(&args, sz, &cap);
    args[sz] = NULL;
    return args;
}

static
void command_error(char *cmd, char **args)
{
    struct stat st;

    if (access(cmd, F_OK)) {
            write(STDERR_FILENO, args[0], u_strlen(args[0]));
            WRITE_CONST(STDERR_FILENO, ": Command not found.\n");
            return;
    }
    stat(cmd, &st);
    if (S_ISDIR(st.st_mode) || access(cmd, X_OK)) {
            write(STDERR_FILENO, args[0], u_strlen(args[0]));
            WRITE_CONST(STDERR_FILENO, ": Permissions denied.\n");
    }
}

static
int launch_bin(char *full_bin_path, char **args, env_t *env, char *buff)
{
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        if (execve(full_bin_path, args, env->env) < 0) {
            command_error(full_bin_path, args);
            free_env(env);
            free((void *)args);
            free(buff);
            exit(RETURN_FAILURE);
        }
    }
    waitpid(pid, &status, 0);
    return status;
}

static
void status_handler(int status, history_t *history)
{
    char *strsig;

    if (WIFEXITED(status))
        history->last_exit_code = WEXITSTATUS(status);
    if (!WIFEXITED(status) && WIFSIGNALED(status)) {
        history->last_exit_code = WTERMSIG(status);
        if (WTERMSIG(status) != SIGFPE) {
            strsig = strsignal(WTERMSIG(status));
            write(STDERR_FILENO, strsig, u_strlen(strsig));
        } else
            WRITE_CONST(STDERR_FILENO, "Floating exception");
        if (WCOREDUMP(status)) {
            history->last_exit_code += 128;
            WRITE_CONST(STDERR_FILENO, " (core dumped)");
        }
        WRITE_CONST(STDERR_FILENO, "\n");
    }
    U_DEBUG("Exit code [%d]\n", history->last_exit_code);
}

static
bool builtins_launcher(char *buffer, env_t *env, history_t *history,
    char **args)
{
    int buffer_l = u_strlen(buffer);

    for (size_t i = 0; i < BUILTINS_SZ; i++) {
        if (u_strlen(BUILTINS[i].name) != buffer_l)
            continue;
        if (u_strcmp(BUILTINS[i].name, buffer) == 0) {
            history->last_exit_code =
                BUILTINS[i].ptr(env, args, buffer, history);
            return true;
        }
    }
    return false;
}

int execute(char *buffer, env_t *env, history_t *history)
{
    char *path = NULL;
    char *full_bin_path;
    char **args = parse_args(buffer);
    int status;

    if (!args)
        return RETURN_FAILURE;
    if (builtins_launcher(buffer, env, history, args))
        return RETURN_SUCCESS;
    path = get_env_value(env, "PATH");
    full_bin_path = find_binary(path, args[0]);
    if (full_bin_path == NULL)
        return (free((void *)args), RETURN_FAILURE);
    U_DEBUG("Found bin [%s]\n", full_bin_path);
    status = launch_bin(full_bin_path, args, env, buffer);
    status_handler(status, history);
    free(full_bin_path);
    free((void *)args);
    return RETURN_SUCCESS;
}
