/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

int swap_uint32(int src)
{
    return ((src & 0xFF000000) >> 24) |
    ((src & 0x00FF0000) >> 8) |
    ((src & 0x0000FF00) << 8) |
    ((src & 0x000000FF) << 24);
}
