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
    int status;

    if (!ef->exec_ctx->isatty)
        return fprintf(stderr, "No job control in this shell.\n"),
            RETURN_FAILURE;
    if (!set_child_fg(ef->exec_ctx, ef->exec_ctx->jobs.sz - 1))
        return RETURN_FAILURE;
    waitpid(ef->exec_ctx->jobs.jobs[ef->exec_ctx->jobs.sz - 1].pgid, &status,
        WUNTRACED);
    if (WIFEXITED(status))
        ef->exec_ctx->history->last_exit_code =
            ef->exec_ctx->history->last_exit_code ?: WEXITSTATUS(status);
    tcsetpgrp(ef->exec_ctx->read_fd, ef->exec_ctx->jobs.jobs[0].pgid);
    return RETURN_SUCCESS;
}
