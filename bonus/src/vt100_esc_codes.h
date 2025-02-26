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

    // move
    #define MOVE_RIGHT(n) ESC "[" #n "C"

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
