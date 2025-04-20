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
#include <string.h>

int check_value(int val, globs_t *globs)
{
    if (val != 0){
        if (val == GLOB_NOMATCH)
            WRITE_CONST(STDERR_FILENO, "No match.\n");
        globfree(&globs->globs);
        return -1;
    }
    return 0;
}

char **globbing(const char *pattern)
{
    globs_t *globs = malloc(sizeof(globs_t));

    if (!globs)
        return NULL;
    globs->val = glob(pattern, GLOB_ERR, NULL, &globs->globs);
    if (check_value(globs->val, globs) == -1)
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
