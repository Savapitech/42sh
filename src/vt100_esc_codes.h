/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef CR_VT100_ESC_CODES_H
    #define CR_VT100_ESC_CODES_H

    #define ESC "\033"
    #define CFMT(n) ESC "[" #n "m"

    // special
    #define BLINKING_VERTICAL_CURSOR ESC "[5 q"

    // move
    #define MOVE_RIGHT(n) ESC "[" #n "C"
    #define MOVE_LEFT(n) ESC "[" #n "D"
    #define MOVE_TO_N_COL(n) ESC "[" #n  "G"

    // delete
    #define DELETE_CHARS(n) ESC "[" #n "P"
    #define ERASE_TO_END_LINE ESC "[0K"

    // colors
    #define RESET CFMT(0)
    #define BOLD CFMT(1)

    #define RED CFMT(31)
    #define GREEN CFMT(32)
    #define YELLOW CFMT(33)
    #define BLUE CFMT(34)
    #define PURPLE CFMT(35)
    #define CYAN CFMT(36)
#endif
