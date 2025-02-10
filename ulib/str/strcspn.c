/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

int u_strcspn(char *str, char c)
{
    char *old_str = str;

    for (; *str != '\0'; str++)
        if (*str == c)
            return str - old_str;
    return 0;
}
