/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef READLINE
    #define READLINE
    #define CTRL(x) (x & 0xf)
    #define BUFF_INIT_SZ 128
    #include <stdbool.h>

    #include "u_str.h"

bool readline(buff_t *buff);
#endif /* READLINE */
