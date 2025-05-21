/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef READLINE
    #define READLINE
    #include <stdbool.h>
    #include <sys/ioctl.h>

    #include "builtins_handler.h"
    #include "u_str.h"

    #define BUFF_INIT_SZ 16
    #define BULK_READ_BUFF_SZ 32

typedef struct {
    exec_ctx_t *ec;
    buff_t *out;
    char *in;
    char cpy[BULK_READ_BUFF_SZ];
    size_t cursor;
    int history_idx;
    struct winsize winsz;
} readline_helper_t;

typedef struct {
    size_t used;
    size_t written;
} text_parse_info_t;

bool readline(exec_ctx_t *ec, buff_t *out);
void write_buff(readline_helper_t *rh);
void refresh_line(readline_helper_t *rh);

// Utils
bool ensure_buff_av_capacity(buff_t *buff, size_t requested);
#endif /* READLINE */
