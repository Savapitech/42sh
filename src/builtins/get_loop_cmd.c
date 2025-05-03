/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** get_loop_cmd
*/

#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "loop.h"
#include "u_mem.h"
#include "u_str.h"

static
usr_cmd_t *buffers_realloc(usr_cmd_t *usr)
{
    char **new_buffers = (char **)u_realloc((void *)usr->cmds, sizeof
        *usr->cmds * usr->sz, sizeof
        *usr->cmds * (usr->cap << 1));

    if ((void *)new_buffers == NULL)
        return nullptr;
    usr->cmds = new_buffers;
    usr->cap <<= 1;
    return usr;
}

static
usr_cmd_t *increase_buffers(usr_cmd_t *usr, size_t *buffer_len)
{
    usr->cmds[usr->sz] = nullptr;
    getline(&(usr->cmds[usr->sz]), buffer_len, stdin);
    *buffer_len = u_strlen(usr->cmds[usr->sz]);
    usr->cmds[usr->sz][*buffer_len - 1] = '\0';
    usr->sz++;
    return usr;
}

static
usr_cmd_t *handle_end(usr_cmd_t *us, char const *prompt)
{
    us->sz--;
    if (!us->cmds[us->sz] || strcmp("end", us->cmds[us->sz]) != 0){
            printf("%s: end not found.\n", prompt);
            free_array(us->cmds);
            us->cmds = nullptr;
            exit(RETURN_FAILURE);
            return nullptr;
        }
        free(us->cmds[us->sz]);
        us->cmds[us->sz] = nullptr;
        return us;
}

static
usr_cmd_t *get_first_cmd(usr_cmd_t *usr, char const *prompt, size_t *bf_len)
{
    if (isatty(STDIN_FILENO))
        printf("%s? ", prompt);
    usr->cmds[usr->sz] = nullptr;
    getline(&(usr->cmds[usr->sz]), bf_len, stdin);
    *bf_len = u_strlen(usr->cmds[usr->sz]);
    usr->cmds[usr->sz][*bf_len - 1] = '\0';
    usr->sz += 1;
    return usr;
}

usr_cmd_t *get_usr_loop_cmd(usr_cmd_t *usr_cmd, char const *prompt)
{
    size_t buffer_len;

    if (usr_cmd == NULL)
        return nullptr;
    usr_cmd->cmds = (char **)malloc(sizeof(char *) * usr_cmd->cap);
    if ((void *)usr_cmd->cmds == NULL)
        return nullptr;
    usr_cmd = get_first_cmd(usr_cmd, prompt, &buffer_len);
    while (strcmp("end", usr_cmd->cmds[usr_cmd->sz - 1]) != 0){
        if (isatty(STDIN_FILENO))
            printf("%s? ", prompt);
        if (usr_cmd->sz >= usr_cmd->cap)
            usr_cmd = buffers_realloc(usr_cmd);
        if (usr_cmd == NULL)
            return nullptr;
        increase_buffers(usr_cmd, &buffer_len);
    }
    usr_cmd = handle_end(usr_cmd, prompt);
    return usr_cmd;
}
