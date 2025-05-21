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

int builtins_jobs(ef_t *ef, char **)
{
    if (!ef->exec_ctx->isatty)
        return RETURN_SUCCESS;
    for (size_t i = 1; i < ef->exec_ctx->jobs.sz; i++)
        dprintf(ef->out_fd, "[%lu]  + %s\t %s.\n", i,
            ef->exec_ctx->jobs.jobs[i].running ? "Running" : "Suspended",
            ef->exec_ctx->jobs.jobs[i].bin_name);
    return RETURN_SUCCESS;
}
