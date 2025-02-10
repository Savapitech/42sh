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

size_t count_env_entries(char **env)
{
    size_t result = 0;

    for (; *env != NULL; env++)
        result++;
    return result;
}

bool ensure_buff_av_capacity(buff_t *buff, size_t requested)
{
    char *new_str;
    size_t endsize = ENV_BUFF_CAP;

    if ((buff->sz + requested) < buff->cap)
        return true;
    for (; endsize < buff->sz + requested; endsize <<= 1);
    if (endsize > buff->cap) {
        new_str = u_realloc(buff->str, (sizeof *buff->str) * buff->sz,
            (sizeof *buff->str) * endsize);
        if (new_str == NULL)
            return false;
        buff->str = new_str;
        buff->cap = endsize;
    }
    return true;
}

static
bool parse_env_populate(char **env, buff_t *env_values,
    env_entry_t *env_entries)
{
    size_t env_size = 0;
    size_t i = 0;

    for (; *env != NULL; env++) {
        env_size = u_strlen(*env);
        if (!ensure_buff_av_capacity(env_values, env_size))
            return false;
        env_entries[i].ptr = u_memcpy(env_values->str + env_values->sz, *env,
            env_size);
        env_entries[i].size = env_size;
        env_values->sz += env_size;
        i++;
    }
    return true;
}

void debug_env_entries(env_entry_t *env_entries, size_t env_size)
{
    size_t keylen __attribute__((unused));
    for (size_t i = 0; i < env_size; i++) {
        keylen = u_strcspn(env_entries[i].ptr, '=') + 1;
        U_DEBUG("Env entry [%01lu] key [%.*s] value [%.*s]\n", i,
            (int)keylen - 1, env_entries[i].ptr,
            (int)(env_entries[i].size - keylen),
            env_entries[i].ptr + keylen);
    }
}

bool parse_env(char **env, buff_t *env_values, env_entry_t *env_entries)
{
    if (env_values == NULL || env_entries == NULL)
        return false;
    u_bzero((char *)env_values, sizeof(buff_t));
    env_values->str = malloc(sizeof *env_values->str * ENV_BUFF_CAP);
    if (env_values->str == NULL)
        return false;
    env_values->cap = ENV_BUFF_CAP;
    u_bzero(env_values->str, sizeof *env_values->str * env_values->cap);
    parse_env_populate(env, env_values, env_entries);
    env_values->str[env_values->sz] = '\0';
    U_DEBUG("Parsed %zu env entries (%zu)\n",
        count_env_entries(env), env_values->sz);
    return true;
}
