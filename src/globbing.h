/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** globbing
*/

#ifndef INCLUDED_GLOBBING_H
    #define INCLUDED_GLOBBING_H
    #include <glob.h>

typedef struct globs_s {
    glob_t globs;
    char **found_tab;
    char **result_tab;
    int val;
}globs_t;

char **globbing(const char *pattern);
#endif
