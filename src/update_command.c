/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** update_command
*/
#include "history.h"
#include "builtins_handler.h"
#include "u_str.h"
#include "common.h"

static int check_cmd(char *cmd)
{
    if (!cmd)
        return 84;
    for (int i = 0; cmd[i] != 0; i++)
        if (cmd[i] == CHAR_HIST &&
            (cmd[i + 1] != ' ' && cmd[i + 1] != '\t'
            && cmd[i + 1] != '\0'))
            return 84;
    return 0;
}

his_command_t *save_command(char *cmd, his_command_t *cmd_history)
{
    if (check_cmd(cmd) == 84)
        return cmd_history;
    if (cmd_history->sz < 100) {
        cmd_history[cmd_history->sz] = set_cmd(cmd,
            cmd_history[cmd_history->sz]);
        }
    cmd_history->sz++;
    return cmd_history;
}

size_t update_command(char **buffer,
    size_t *buffer_sz, exec_ctx_t *exec_ctx)
{
    size_t buffer_len = 0;

    buffer_len = u_strlen(*buffer);
    if (buffer_len < 2)
        return RETURN_FAILURE;
    if (parse_history(buffer, &buffer_len,
        buffer_sz, &exec_ctx->history_command) == 84)
        return RETURN_SUCCESS;
    parse_alias(buffer, &buffer_len, exec_ctx->alias);
    exec_ctx->history_command = save_command(*buffer,
        exec_ctx->history_command);
    return buffer_len;
}
