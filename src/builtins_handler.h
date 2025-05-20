/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#ifndef BUILTINS_HANDLER_H
    #define BUILTINS_HANDLER_H
    #include <termios.h>

    #include "alias.h"
    #include "env.h"
    #include "history.h"
    #include "local.h"
    #include "shell.h"

typedef struct {
    pid_t pgid;
    bool running;
    bool foreground;
} job_t;

typedef struct {
    job_t *jobs;
    size_t sz;
    size_t cap;
} jobs_t;

typedef struct {
    env_t *env;
    history_t *history;
    his_command_t *history_command;
    alias_t *alias;
    bool is_running;
    struct termios saved_term_settings;
    local_t *local;
    opt_t *opt;
    int read_fd;
    int isatty;
    size_t prompt_len;
    char *precmd;
    char *cwdcmd;
    bool ignoreof;
    jobs_t jobs;
} exec_ctx_t;

size_t update_command(char **buffer,
    size_t *buffer_sz, exec_ctx_t *exec_ctx);
#endif /* BUILTINS_HANDLER_H */
