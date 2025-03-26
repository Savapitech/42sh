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
char *find_binary(const char *path_env, const char *binary, char **dup_path)
{
    static char *saveptr = NULL;
    char *token;
    char *full_path;

    if (path_env) {
        *dup_path = u_strdup(path_env);
        if (!*dup_path)
            return NULL;
        token = strtok_r(*dup_path, ":", &saveptr);
    } else
        token = strtok_r(NULL, ":", &saveptr);
    if (!token)
        return u_strdup(binary);
    full_path = build_full_path(token, binary);
    if (!full_path)
        return NULL;
    return access(full_path, X_OK) == 0 ? full_path : (free(full_path),
        find_binary(NULL, binary, dup_path));
}

char *parse_full_bin_path(env_t *env, char *bin_name)
{
    char const *path = get_env_value(env, "PATH");
    char *dup_path = NULL;
    char *full_bin_path;

    if (path == NULL)
        path = DEFAULT_PATH;
    U_DEBUG("Used path [%s]\n", path);
    full_bin_path = find_binary(path, bin_name, &dup_path);
    U_DEBUG("Exec bin [%s]\n", full_bin_path);
    if (full_bin_path == NULL)
        return NULL;
    free(dup_path);
    return full_bin_path;
}
