/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef KEY_HANDLER_H
    #define KEY_HANDLER_H

    #include "builtins_handler.h"
    #include "readline.h"
    #include "u_str.h"

typedef struct {
    const char *name;
    bool (*exec)(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
} key_handler_t;

// CTRL
bool handle_key_ctrl_c(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_d(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_l(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_e(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_a(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_b(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_f(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_ctrl_k(readline_helper_t *rh, exec_ctx_t *, buff_t *buff);
bool handle_key_ctrl_w(readline_helper_t *rh, exec_ctx_t *, buff_t *buff);
bool handle_key_ctrl_y(readline_helper_t *rh, exec_ctx_t *, buff_t *buff);
bool handle_backspace(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_delete(readline_helper_t *rh, exec_ctx_t *, buff_t *buff);

// Arrows
bool handle_key_arrow_up(readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_arrow_left(
    readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_arrow_right(
    readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);
bool handle_key_arrow_down(
    readline_helper_t *rh, exec_ctx_t *ec, buff_t *buff);

#endif
