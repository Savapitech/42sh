/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "u_mem.h"
#include "u_str.h"

char *get_env_value(env_t *env, char const *key)
{
    int key_len = u_strlen(key);

    for (size_t i = 0; i < env->sz; i++) {
        if (env->env[i] == NULL)
            continue;
        if (u_strcspn(env->env[i], '=') != key_len)
            continue;
        if (u_strcmp(env->env[i], key) == 0)
            return env->env[i] + key_len + 1;
    }
    return NULL;
}

static
void unset_env_move(env_t *env, size_t i)
{
    while (env->env[i]) {
        env->env[i] = env->env[i + 1];
        i++;
    }
}

bool unset_env(env_t *env, char *key)
{
    int key_len = u_strlen(key);

    for (size_t i = 0; i < env->sz; i++) {
        if (env->env[i] == NULL)
            continue;
        if (u_strcspn(env->env[i], '=') != key_len)
            continue;
        if (u_strcmp(env->env[i], key) == 0) {
            unset_env_move(env, i);
            env->sz--;
            return true;
        }
    }
    return false;
}

void free_env(env_t *env)
{
    for (size_t i = 0; i < env->sz; i++) {
        if (env->env[i] == NULL)
            continue;
        free(env->env[i]);
    }
    free((void *)env->env);
}

static __attribute__((nonnull))
bool ensure_env_capacity(env_t *env)
{
    char **new_ptr;

    if (env->sz < env->cap)
        return true;
    new_ptr = (char **)u_realloc((void *)env->env, sizeof *env->env * env->sz,
        sizeof *env->env * env->cap << 1);
    if (!new_ptr)
        return false;
    env->cap <<= 1;
    env->env = new_ptr;
    return true;
}

static
void env_bzero(char **env, size_t sz)
{
    for (size_t i = 0; i < sz; i++)
        env[i] = NULL;
}

bool set_env(env_t *env, char *key, char *value)
{
    char *new_env = NULL;
    size_t key_len = u_strlen(key);
    size_t value_len = u_strlen(value);

    if (get_env_value(env, key) != NULL)
        unset_env(env, key);
    env->sz++;
    if (!ensure_env_capacity(env))
        return false;
    new_env = malloc(sizeof(char) * (key_len + value_len + 2));
    if (new_env == NULL)
        return false;
    u_bzero(new_env, key_len + value_len + 2);
    u_strcpy(new_env, key);
    new_env[key_len] = '=';
    if (value_len > 0)
        u_strcpy(new_env + key_len + 1, value);
    env->env[env->sz - 1] = new_env;
    return true;
}

env_t parse_env(char **env)
{
    env_t new_env = { 0, .cap = BASE_ENV_CAP };

    new_env.env = (char **)malloc(sizeof(char *) * new_env.cap);
    if (!new_env.env)
        return (env_t){ 0, .env = NULL };
    env_bzero(new_env.env, new_env.sz);
    for (; *env != NULL; env++) {
        if (!ensure_env_capacity(&new_env))
            return (free((void *)new_env.env), (env_t){ 0 });
        new_env.env[new_env.sz] = u_strdup(*env);
        if (new_env.env[new_env.sz] == NULL)
            return (free((void *)new_env.env), (env_t){ 0 });
        new_env.sz++;
    }
    if (!ensure_env_capacity(&new_env))
        return (free((void *)new_env.env), (env_t){ 0 });
    new_env.env[new_env.sz] = NULL;
    return new_env;
}
