/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef ENV_H
    #define ENV_H
    #include <stdbool.h>
    #include <stddef.h>
    #define BASE_ENV_CAP 128

typedef struct {
    size_t sz;
    size_t cap;
    char **env;
} env_t;

__attribute__((unused))
void free_env(env_t *env);
__attribute__((unused))
env_t parse_env(char **env);
__attribute__((unused))
char *get_env_value(env_t *env, char const *key);
__attribute__((unused))
bool unset_env(env_t *env, char *key);
__attribute__((nonnull(1, 2)))
bool set_env(env_t *env, char *key, char *value);
#endif
