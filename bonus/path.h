/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef PATH_H
    #define PATH_H
    #include "env.h"

    #define DEFAULT_PATH "/usr/bin:."

char *parse_full_bin_path(env_t *env, char *bin_name);
#endif
