/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <unistd.h>

#include "key_handler.h"

bool handle_key_arrow_up(exec_ctx_t *ec, buff_t *buff)
{
    WRITE_CONST(STDOUT_FILENO, "up\n");
    return false;
}

bool handle_key_arrow_left(exec_ctx_t *ec, buff_t *buff)
{
    WRITE_CONST(STDOUT_FILENO, "left\n");
    return false;
}

bool handle_key_arrow_right(exec_ctx_t *ec, buff_t *buff)
{
    WRITE_CONST(STDOUT_FILENO, "right\n");
    return false;
}

bool handle_key_arrow_down(exec_ctx_t *ec, buff_t *buff)
{
    WRITE_CONST(STDOUT_FILENO, "down\n");
    return false;
}
