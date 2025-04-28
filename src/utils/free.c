/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <stdlib.h>

#include "exec.h"

void free_everything(exec_ctx_t *exec_ctx)
{
    free_env(exec_ctx->env);
    free_alias(exec_ctx->alias);
    free(exec_ctx->history_command);
}
