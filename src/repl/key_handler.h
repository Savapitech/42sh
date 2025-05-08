/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef KEY_HANDLER_H
    #define KEY_HANDLER_H

    #include "builtins_handler.h"
    #include "u_str.h"

typedef struct {
    const char *name;
    bool (*exec)(exec_ctx_t *ec, buff_t *buff);
} key_handler_t;

bool handle_key_ctrl_c(exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_d(exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_l(exec_ctx_t *ec, buff_t *buff);
bool handle_delete(exec_ctx_t *ec, buff_t *buff);

bool handle_key_arrow_up(exec_ctx_t *ec, buff_t *buff);
bool handle_key_arrow_left(exec_ctx_t *ec, buff_t *buff);
bool handle_key_arrow_right(exec_ctx_t *ec, buff_t *buff);
bool handle_key_arrow_down(exec_ctx_t *ec, buff_t *buff);

#endif
