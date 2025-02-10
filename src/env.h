/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef ENV_H
    #define ENV_H
    #include "u_str.h"
    #define ENV_BUFF_CAP 128

typedef struct {
    char *ptr;
    size_t size;
} env_entry_t;

// Debug
void debug_env_entries(env_entry_t *env_entries, size_t env_size);

bool parse_env(char **env, buff_t *env_values, env_entry_t *env_entries);
size_t count_env_entries(char **env);
#endif
