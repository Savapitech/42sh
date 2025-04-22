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

char **globbing(const char *pattern, char ***args);
int process_glob_results(char **glob_results, char ***args,
    size_t *sz, size_t *cap);
int process_args(char *arg, char ***args, size_t *sz, size_t *cap);
#endif
