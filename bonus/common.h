/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef COMMON_H
    #define COMMON_H
    #include <stdint.h>

    #define PROMPT_HEADER "┌─["
    #define IF_PROMPT "if? "

enum {
    RETURN_SUCCESS = 0,
    RETURN_FAILURE = 1
};

typedef struct {
    uint8_t flags;
    char *script_file;
    char *cmd;
} opt_t;
#endif /* COMMON_H */
