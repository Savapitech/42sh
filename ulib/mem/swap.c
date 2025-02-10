/*
** EPITECH PROJECT, 2024
** CPoolDay07
** File description:
** ./u_swap.c
*/

void u_swap(int *a, int *b)
{
    int swap;

    swap = *a;
    *a = *b;
    *b = swap;
}
