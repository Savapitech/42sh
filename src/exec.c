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

#include "common.h"
#include "debug.h"
#include "env.h"
#include "exec.h"
#include "u_mem.h"
#include "u_str.h"

static
const builtins_t BUILTINS[] = {
    { "exit", (void (*)(uint64_t))&exit }
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

static
int count_args(char *buffer)
{
    int count = 0;
    char *token;

    token = strtok(buffer, " \t\n");
    while (token != NULL) {
        count++;
        token = strtok(NULL, " \t\n");
    }
    return count;
}

static
void free_args(char **args)
{
    char **old_args = args;

    for (; *args != NULL; args++)
        free(*args);
    free((void *)old_args);
}

static
char **parse_args(char *buffer)
{
    int i = 0;
    int arg_count = count_args(buffer);
    char **args = (char **)malloc((arg_count + 1) * sizeof(char *));
    char *token;

    if (!args)
        return NULL;
    token = strtok(buffer, " ");
    while (token != NULL) {
        args[i] = (char *)malloc(u_strlen(token) + 1);
        u_bzero(args[i], u_strlen(token) + 1);
        if (args[i] == NULL)
            return (free((void *)args), NULL);
        u_strcpy(args[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

static
int launch_bin(char *full_bin_path, char **args, char **env)
{
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        if (execve(full_bin_path, args, env) < 0) {
            WRITE_CONST(STDERR_FILENO, "Command not found.\n");
            exit(127);
        }
    }
    waitpid(pid, &status, 0);
    return status;
}

int execute(char *buffer, env_t *env)
{
    char *path = NULL;
    char *full_bin_path;
    char **args = parse_args(buffer);

    if (!args)
        return RETURN_FAILURE;
    for (size_t i = 0; i < BUILTINS_SZ; i++)
        if (u_strcmp(buffer, BUILTINS[i].name) == 0)
            BUILTINS[i].ptr(0);
    buffer[u_strlen(buffer) - 1] = '\0';
    path = get_env_value(env, "PATH");
    full_bin_path = find_binary(path, args[0]);
    U_DEBUG("Found bin [%s]\n", full_bin_path);
    if (full_bin_path == NULL)
        return (free_args(args), RETURN_FAILURE);
    launch_bin(full_bin_path, args, NULL);
    free(full_bin_path);
    free_args(args);
    return 0;
}
