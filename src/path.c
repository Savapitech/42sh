/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "env.h"
#include "path.h"
#include "u_str.h"

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

char *parse_full_bin_path(env_t *env, char *bin_name)
{
    char const *path = get_env_value(env, "PATH");
    char *full_bin_path;

    if (path == NULL)
        path = DEFAULT_PATH;
    U_DEBUG("Used path [%s]\n", path);
    full_bin_path = find_binary(path, bin_name);
    U_DEBUG("Exec bin [%s]\n", full_bin_path);
    if (full_bin_path == NULL)
        return NULL;
    return full_bin_path;
}
