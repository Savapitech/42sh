/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>
#include <unistd.h>

#include "builtins_handler.h"
#include "exec.h"
#include "job.h"

void set_ignored_signals(int child)
{
    signal(SIGINT, child ? SIG_DFL : SIG_IGN);
    signal(SIGQUIT, child ? SIG_DFL : SIG_IGN);
    signal(SIGTSTP, child ? SIG_DFL : SIG_IGN);
    signal(SIGTTIN, child ? SIG_DFL : SIG_IGN);
    signal(SIGTTOU, child ? SIG_DFL : SIG_IGN);
}

bool ensure_jobs_capacity(jobs_t *jobs)
{
    job_t *tmp;

    if (jobs->sz + 1 < jobs->cap)
        return true;
    tmp = realloc(jobs->jobs, sizeof *jobs->jobs * jobs->cap << 1);
    if (tmp == nullptr)
        return false;
    jobs->jobs = tmp;
    jobs->cap <<= 1;
    return true;
}

bool set_child_term(exec_ctx_t *ec, size_t idx)
{
    return tcsetpgrp(ec->read_fd, ec->jobs.jobs[idx].pgid) < 0 ? false : true;
}

static
bool init_child_parent(exec_ctx_t *ec, pid_t pid, ef_t *ef)
{
    if (!ef->bg) {
        setpgid(pid, pid);
        tcsetpgrp(ec->read_fd, pid);
    }
    if (!ensure_jobs_capacity(&ec->jobs))
        return false;
    ec->jobs.jobs[ec->jobs.sz].pgid = pid;
    ec->jobs.jobs[ec->jobs.sz].running = true;
    ec->jobs.jobs[ec->jobs.sz].foreground = !ef->bg;
    ec->jobs.jobs[ec->jobs.sz].bin_name =
        ec->history_command[ec->history_command->sz - 1].command;
    ec->jobs.sz++;
    return true;
}

bool init_child_job(exec_ctx_t *ec, pid_t pid, ef_t *ef)
{
    if (!ec->isatty)
        return true;
    if (pid == 0) {
        setpgid(0, 0);
        if (!ef->bg && tcsetpgrp(ec->read_fd, getpid()) < 0)
            return false;
        set_ignored_signals(1);
        return true;
    }
    return init_child_parent(ec, pid, ef);
}

bool init_jobs(exec_ctx_t *ec)
{
    ec->jobs.jobs = nullptr;
    if (!isatty(ec->read_fd))
        return true;
    ec->jobs.cap = DEFAULT_JOBS_CAP;
    ec->jobs.jobs = malloc(sizeof *ec->jobs.jobs * ec->jobs.cap);
    if (ec->jobs.jobs == nullptr)
        return false;
    ec->jobs.jobs[0].pgid = getpid();
    ec->jobs.jobs[0].running = true;
    ec->jobs.sz = 1;
    if (setpgid(ec->jobs.jobs[0].pgid, ec->jobs.jobs[0].pgid) < 0)
        return false;
    if (tcsetpgrp(ec->read_fd, ec->jobs.jobs[0].pgid) < 0)
        return false;
    set_ignored_signals(0);
    return true;
}
