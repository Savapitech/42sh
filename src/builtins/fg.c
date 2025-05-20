/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <unistd.h>

#include "common.h"
#include "exec.h"
#include "job.h"

int builtins_fg(ef_t *ef, char **)
{
    int status = 0;
    int last_job_index = ef->exec_ctx->jobs.sz - 1;

    if (!ef->exec_ctx->isatty) {
        fprintf(stderr, "No job control in this shell.\n");
        return RETURN_FAILURE;
    }
    if (!set_child_term(ef->exec_ctx, last_job_index))
        return RETURN_FAILURE;
    kill(-ef->exec_ctx->jobs.jobs[last_job_index].pgid, SIGCONT);
    waitpid(-ef->exec_ctx->jobs.jobs[last_job_index].pgid, &status, WUNTRACED);
    if (WIFEXITED(status)) {
        ef->exec_ctx->history->last_exit_code =
            ef->exec_ctx->history->last_exit_code ?:
            WEXITSTATUS(status);
    }
    tcsetpgrp(ef->exec_ctx->read_fd, getpid());
    return RETURN_SUCCESS;
}
