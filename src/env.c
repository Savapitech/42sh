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

#include "debug.h"
#include "env.h"
#include "u_mem.h"
#include "u_str.h"

__attribute__((nonnull))
void debug_env_entries(env_t *env)
{
    for (size_t i = 0; i < env->sz; i++)
        U_DEBUG("Env entry [%lu] [%s]\n", i, env->env[i]);
}

__attribute__((nonnull))
char *get_env_value(env_t *env, char const *key)
{
    for (size_t i = 0; i < env->sz; i++)
        if (u_strncmp(env->env[i], key, u_strlen(key)) == 0)
            return env->env[i] + u_strlen(key) + 1;
    return NULL;
}

__attribute__((nonnull))
bool unset_env(env_t *env, char *key)
{
    for (size_t i = 0; i < env->sz; i++) {
        if (u_strncmp(env->env[i], key, u_strlen(key)) == 0) {
            env->env[i] = NULL;
            return true;
        }
    }
    return false;
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
