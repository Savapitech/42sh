/*
** EPITECH PROJECT, 2024
** B-CPE-100-REN-1-1-cpoolday03-savinien.petitjean
** File description:
** u_getnbr.c
*/

long u_pow(int pow)
{
    long r = 1;

    for (long i = 0; i < pow; i++)
        r *= 10;
    return r;
}

int u_atoi(char *str, int size, int negativ, int rsize)
{
    long result = 0;

    if (size > 10 || size == 0 || rsize == 0)
        return 0;
    for (int i = 0; i < size; i++)
        result += u_pow(size - i - 1) * (str[i] - '0');
    if (negativ % 2 != 0)
        result *= (-1);
    if (result > 2147483647 || result < -2147483648)
        return 0;
    return (int)result;
}

int u_getnbr(char const *str)
{
    int start = 0;
    int end = 0;
    int sig = 0;
    char nstr[1000000];
    int size = 0;
    int rsize = 1;

    for (; str[start] < 48 || str[start] > 57; start++);
    end = start + 1;
    for (; str[end] >= 48 && str[end] <= 57; end++);
    for (int i = 0; i < end; i++)
        if (str[i] == '-')
            sig++;
    for (int i = start; i < end; i++) {
        nstr[size] = str[i];
        size++;
    }
    return u_atoi(nstr, size, sig, rsize);
}
