/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef ENV_H
    #define ENV_H
    #include <stddef.h>
    #define BASE_ENV_CAP 128

typedef struct {
    size_t sz;
    size_t cap;
    char **env;
} env_t;

// Debug
void debug_env_entries(env_t *env);

env_t parse_env(char **env);
char *get_env_value(env_t *env, char const *key);
#endif
