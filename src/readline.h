/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef READLINE
    #define READLINE
    #define BUFF_INIT_SZ 16
    #include <stdbool.h>

    #include "exec.h"
    #include "u_str.h"

    #define BULK_READ_BUFF_SZ 32

bool readline(exec_ctx_t *exec_ctx, buff_t *out);

typedef struct {
    buff_t *out;
    char *in;
    char cpy[BULK_READ_BUFF_SZ];
} readline_helper_t;

typedef struct {
    size_t used;
    size_t written;
} text_parse_info_t;

#endif /* READLINE */
