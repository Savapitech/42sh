/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** globbing
*/
#include "globbing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "u_str.h"
#include "exec.h"
#include <string.h>

int check_value(int val, globs_t *globs, char ***args)
{
    if (val != 0){
        if (val == GLOB_NOMATCH){
            write(2, *args[0], strlen(*args[0]));
            WRITE_CONST(STDERR_FILENO, ": No match.\n");
        }
        globfree(&globs->globs);
        return -1;
    }
    return 0;
}

char **globbing(const char *pattern, char ***args)
{
    globs_t *globs = malloc(sizeof(globs_t));

    if (!globs)
        return NULL;
    globs->val = glob(pattern, GLOB_ERR, NULL, &globs->globs);
    if (check_value(globs->val, globs, args) == -1)
        return NULL;
    globs->result_tab = malloc(sizeof(char *) * (globs->globs.gl_pathc + 1));
    globs->found_tab = globs->globs.gl_pathv;
    if (globs->found_tab == NULL || !globs->result_tab){
        globfree(&globs->globs);
        return NULL;
    }
    for (int i = 0; globs->found_tab[i] != NULL; i++)
        globs->result_tab[i] = strdup(globs->found_tab[i]);
    globs->result_tab[globs->globs.gl_pathc] = NULL;
    globfree(&globs->globs);
    return globs->result_tab;
}

int process_glob_results(char **glob_results, char ***args,
    size_t *sz, size_t *cap)
{
    if (!glob_results)
        return -1;
    for (int j = 0; glob_results[j] != NULL; j++) {
        ensure_args_capacity(args, *sz, cap);
        (*args)[*sz] = strdup(glob_results[j]);
        (*sz)++;
        free(glob_results[j]);
    }
    free(glob_results);
    return 0;
}

int process_args(char *arg, char ***args, size_t *sz, size_t *cap)
{
    if (strchr(arg, '*')) {
        if (process_glob_results(globbing(arg, args), args, sz, cap) == -1) {
            free(*args);
            return -1;
        }
    } else {
        ensure_args_capacity(args, *sz, cap);
        (*args)[*sz] = arg;
        (*sz)++;
    }
    return 0;
}
