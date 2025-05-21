/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "git.h"

static
int read_head_branch(char *branch, size_t size)
{
    FILE *f = fopen(".git/HEAD", "r");
    char line[MAX_LINE];

    if (!f)
        return -1;
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return -1;
    }
    fclose(f);
    if (strncmp(line, "ref: refs/heads/", 16) != 0)
        return -1;
    strncpy(branch, line + 16, size - 1);
    branch[strcspn(branch, "\n")] = '\0';
    return 0;
}

static
int read_git_ref(const char *path, char *hash, size_t size)
{
    FILE *f = fopen(path, "r");

    if (!f)
        return -1;
    if (!fgets(hash, size, f)) {
        fclose(f);
        return -1;
    }
    fclose(f);
    hash[strcspn(hash, "\n")] = '\0';
    return 0;
}

static
int compare_hashes(const char *local, const char *remote)
{
    if (strcmp(local, remote) == 0)
        return 0;
    return 1;
}

static
int analyze_divergence(git_status_t *status)
{
    if (compare_hashes(status->local_hash, status->remote_hash) == 0) {
        status->ahead = 0;
        status->behind = 0;
    } else {
        status->ahead = 1;
        status->behind = 1;
    }
    return 0;
}

bool get_git_status(git_status_t *status)
{
    char local_path[256];
    char remote_path[256];

    if (read_head_branch(status->branch, sizeof(status->branch)) != 0)
        return false;
    snprintf(local_path, sizeof(local_path),
        ".git/refs/heads/%s", status->branch);
    snprintf(remote_path, sizeof(remote_path),
            ".git/refs/remotes/origin/%s", status->branch);
    if (read_git_ref(local_path, status->local_hash,
                sizeof(status->local_hash)) != 0)
        return false;
    if (read_git_ref(remote_path, status->remote_hash,
                sizeof(status->remote_hash)) != 0)
        return false;
    analyze_divergence(status);
    return true;
}
