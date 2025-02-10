/*
** EPITECH PROJECT, 2024
** B-CPE-100-REN-1-1-cpoolday06-savinien.petitjean
** File description:
** Task 2
*/

__attribute__((nonnull(1, 2)))
char *u_strncpy(char *dest, char const *src, int n)
{
    int i = 0;

    for (; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    if (src[i] == '\0' && i < n)
        dest[i] = '\0';
    return dest;
}
