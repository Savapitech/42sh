/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef JOB_H
    #define JOB_H
    #include <sys/wait.h>

    #include "builtins_handler.h"
    #define DEFAULT_JOBS_CAP 4

bool ensure_jobs_capacity(jobs_t *jobs);
bool init_jobs(exec_ctx_t *ec);
bool init_child_job(exec_ctx_t *ec, pid_t pid);
bool set_child_fg(exec_ctx_t *ec, size_t idx);
void set_ignored_signals(int child);
#endif /* JOB_H */
