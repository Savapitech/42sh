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

int builtins_bg(ef_t *ef, char **args)
{
    size_t job_idx = ef->exec_ctx->jobs.sz - 1;
    char *end_ptr;

    if (!ef->exec_ctx->isatty) {
        fprintf(stderr, "No job control in this shell.\n");
        return RETURN_FAILURE;
    }
    if (args[1] != nullptr) {
        job_idx = strtol(args[1], &end_ptr, 10);
        if (*end_ptr != '\0' || !job_idx
            || job_idx > ef->exec_ctx->jobs.sz - 1)
            return fprintf(stderr, "%s: No such job.\n", args[0]),
                RETURN_FAILURE;
    }
    ef->exec_ctx->jobs.jobs[job_idx].running = true;
    kill(-ef->exec_ctx->jobs.jobs[job_idx].pgid, SIGCONT);
    return RETURN_SUCCESS;
}
