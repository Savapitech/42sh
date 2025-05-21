/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "exec.h"
#include "job.h"

static
bool get_job_idx(ef_t *ef, char **args, size_t *job_idx)
{
    char *end_ptr;

    if (args[1] != nullptr) {
        *job_idx = strtol(args[1], &end_ptr, 10);
        if (*end_ptr != '\0' || !*job_idx
            || *job_idx > ef->exec_ctx->jobs.sz - 1)
            return fprintf(stderr, "%s: No such job.\n", args[0]), false;
    }
    return true;
}

int builtins_fg(ef_t *ef, char **args)
{
    size_t job_idx = ef->exec_ctx->jobs.sz - 1;
    int status;

    if (!ef->exec_ctx->isatty) {
        fprintf(stderr, "No job control in this shell.\n");
        return RETURN_FAILURE;
    }
    if (!get_job_idx(ef, args, &job_idx))
        return RETURN_FAILURE;
    if (!set_child_term(ef->exec_ctx, job_idx))
        return RETURN_FAILURE;
    kill(-ef->exec_ctx->jobs.jobs[job_idx].pgid, SIGCONT);
    waitpid(-ef->exec_ctx->jobs.jobs[job_idx].pgid, &status, WUNTRACED);
    if (WIFEXITED(status)) {
        ef->exec_ctx->history->last_exit_code =
            ef->exec_ctx->history->last_exit_code ?:
            WEXITSTATUS(status);
    }
    tcsetpgrp(ef->exec_ctx->read_fd, getpid());
    return RETURN_SUCCESS;
}
