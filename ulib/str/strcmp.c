/*
** EPITECH PROJECT, 2024
** B-CPE-100-REN-1-1-cpoolday06-savinien.petitjean
** File description:
** Task 1
*/

int u_strcmp(char const *s1, char const *s2)
{
    int i = 0;

    for (; s1[i] == s2[i] && s1[i] != '\0'; i++);
    if (i > 0 && (s1[i] == '\0' || s2[i] == '\0'))
        return s1[i - 1] - s2[i - 1];
    return s1[i] - s2[i];
}

int u_strncmp(char const *s1, char const *s2, int nb)
{
    int i = 0;

    if (nb == 0)
        return 0;
    for (; s1[i] == s2[i] && s1[i] != '\0' && i < nb - 1; i++);
    return s1[i] - s2[i];
}
