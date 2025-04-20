/*
** EPITECH PROJECT, 2024
** Semester_1
** File description:
** my_array_len
*/
#include <unistd.h>

int my_array_len(char **tab)
{
    int count = 0;

    if (tab == NULL)
        return count;
    for (; tab[count] != NULL; count++);
    return count;
}
