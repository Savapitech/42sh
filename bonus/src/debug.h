/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef DEBUG_H
    #define DEBUG_H
    #include "vt100_esc_codes.h"
    #include <stdio.h>

    #define OMIT

    #ifdef U_DEBUG_MODE
        #define HEAD __FILE_NAME__, __LINE__

        #define HEAD_FMT_FILE BOLD BLUE "%s" RESET
        #define HEAD_FMT_LINE ":" BOLD PURPLE "%d" RESET

        #define HEAD_FMT HEAD_FMT_FILE HEAD_FMT_LINE "  "

        #define ERR(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
        #define DEBUG_INTERNAL(fmt, ...) ERR(HEAD_FMT fmt, HEAD, __VA_ARGS__)

        #define U_DEBUG(fmt, ...) DEBUG_INTERNAL(fmt, __VA_ARGS__)
        #define U_DEBUG_MSG(msg) DEBUG_INTERNAL("%s\n", msg)

        #define U_DEBUG_CALL(func, ...) func(__VA_ARGS__)
    #else
        #define U_DEBUG_CALL(func, ...) OMIT
        #define U_DEBUG_MSG(msg) OMIT
        #define U_DEBUG(fmt, ...) OMIT
    #endif

#endif /* DEBUG_H */
