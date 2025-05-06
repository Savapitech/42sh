/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef STRING_H
    #define STRING_H
    #include <stdbool.h>
    #include <stddef.h>
    #define WRITE_CONST(fd, str) write(fd, str, sizeof str - 1)

typedef struct {
    char *str;
    size_t sz;
    size_t cap;
} buff_t;

int u_getnbr(char const *);
char *u_strcat(char *, char const *);
char *u_strncat(char *, char const *, int);
char *u_strdup(char const *);
int u_strlen(char const *);
char *u_strcpy(char *, char const *);
char *u_strncpy(char *, char const *, int);
char *u_revstr(char *);
char *u_strstr(char *, char const *);
int u_strcmp(char const *, char const *);
int u_strncmp(char const *, char const *, int);
char *u_numstr(char *, int);
int u_spacelen(char const *str);
int u_strcspn(char *str, char c);
bool u_str_is_alnum(char *str);
bool u_str_is_only_alnum(char *str);
void free_array(char **array);
void puterror(char const *prefix);
int my_array_len(char **tab);
char **arraydup(char **array);
char *u_strnchr(char *str, char to_search, size_t n);

#endif /* STRING_H */
