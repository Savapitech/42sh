/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <ctype.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readline.h"

static
buff_t get_current_word(readline_helper_t *rh, buff_t *buff)
{
    buff_t word = {buff->str, .sz = rh->cursor};

    for (size_t i = rh->cursor; i; i--) {
        if (isblank(buff->str[i])) {
            word.str = &buff->str[i + 1];
            word.sz = rh->cursor - (i + 1);
            break;
        }
    }
    return word;
}

bool handle_key_tab(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff)
{
    buff_t word;
    glob_t globs;
    char *pattern;

    if (!rh->cursor || !buff->sz)
        return false;
    printf("TAB\n");
    word = get_current_word(rh, buff);
    printf("Current word [%.*s]\n", (int)word.sz, word.str);
    pattern = malloc(sizeof *pattern * (word.sz + 2));
    if (pattern == nullptr)
        return true;
    strncpy(pattern, word.str, word.sz);
    strcpy(pattern + word.sz, "*\0");
    printf("Gen pattern [%s]\n", pattern);
    if (glob(pattern, GLOB_ERR, nullptr, &globs) != 0)
        return false;
    for (size_t i = 0; i < globs.gl_pathc; i++) {
        printf("Path v [%s] [%zu]\n", globs.gl_pathv[i], i);
    }
    if (!ensure_buff_av_capacity(buff, strlen(globs.gl_pathv[0])))
        return true;
    strcpy(word.str, globs.gl_pathv[0]);
    buff->sz += strlen(globs.gl_pathv[0]) - word.sz;
    rh->cursor = buff->sz;
    refresh_line(rh);
    free(pattern);
    return false;
}
