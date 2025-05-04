/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef COMMON_H
    #define COMMON_H
    #include "exec.h"

    #define IF_PROMPT "if? "

enum {
    RETURN_SUCCESS = 0,
    RETURN_FAILURE = 1
};

void free_everything(exec_ctx_t *exec_ctx);
#endif /* COMMON_H */
