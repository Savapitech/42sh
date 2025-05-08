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

#include "loop.h"
#include "readline.h"
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
bool increase_buffers(
    exec_ctx_t *exec_ctx, usr_cmd_t *usr, size_t *buffer_len)
{
    buff_t buff = { .str = nullptr, .sz = 0, .cap = 0 };

    if (!readline(exec_ctx, &buff))
        return false;
    if (!buff.sz)
        return false;
    usr->cmds[usr->sz] = buff.str;
    *buffer_len = u_strlen(usr->cmds[usr->sz]);
    usr->sz++;
    return true;
}

static
usr_cmd_t *handle_end(usr_cmd_t *us, char const *prompt)
{
    us->sz--;
    if (!us->cmds[us->sz] || strcmp("end", us->cmds[us->sz]) != 0){
        printf("%s: end not found.\n", prompt);
        free_array(us->cmds);
        us->cmds = nullptr;
        return nullptr;
    }
    free(us->cmds[us->sz]);
    us->cmds[us->sz] = nullptr;
    return us;
}

static
usr_cmd_t *get_first_cmd(exec_ctx_t *exec_ctx, usr_cmd_t *usr,
    char const *prompt, size_t *bf_len)
{
    buff_t buff = { .str = nullptr, .sz = 0, .cap = 0 };

    if (isatty(exec_ctx->read_fd))
        printf("%s? ", prompt);
    if (!readline(exec_ctx, &buff))
        return nullptr;
    if (!buff.sz)
        return nullptr;
    usr->cmds[usr->sz] = buff.str;
    *bf_len = u_strlen(usr->cmds[usr->sz]);
    usr->sz++;
    return usr;
}

usr_cmd_t *get_usr_loop_cmd(exec_ctx_t *exec_ctx, usr_cmd_t *usr_cmd,
    char const *prompt)
{
    size_t buffer_len;

    usr_cmd->cmds = (char **)malloc(sizeof(char *) * usr_cmd->cap);
    if ((void *)usr_cmd->cmds == NULL)
        return nullptr;
    usr_cmd = get_first_cmd(exec_ctx, usr_cmd, prompt, &buffer_len);
    if (usr_cmd == nullptr)
        return nullptr;
    while (strcmp("end", usr_cmd->cmds[usr_cmd->sz - 1]) != 0){
        if (isatty(exec_ctx->read_fd))
            printf("%s? ", prompt);
        if (usr_cmd->sz >= usr_cmd->cap)
            usr_cmd = buffers_realloc(usr_cmd);
        if (usr_cmd == NULL ||
                !increase_buffers(exec_ctx, usr_cmd, &buffer_len))
            return nullptr;
    }
    usr_cmd = handle_end(usr_cmd, prompt);
    return usr_cmd;
}
