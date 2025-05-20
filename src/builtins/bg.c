/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "exec.h"

int builtins_bg(ef_t *ef, char **)
{
    int last_job_index = ef->exec_ctx->jobs.sz - 1;

    if (!ef->exec_ctx->isatty) {
        fprintf(stderr, "No job control in this shell.\n");
        return RETURN_FAILURE;
    }
    kill(-ef->exec_ctx->jobs.jobs[last_job_index].pgid, SIGCONT);
    return RETURN_SUCCESS;
}
