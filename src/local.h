/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef LOCAL_H
    #define LOCAL_H
    #include <stdbool.h>
    #include <stddef.h>

typedef struct {
    size_t sz;
    size_t cap;
    char **local_var;
    bool in_a_loop;
} local_t;

bool set_local(local_t *local, char *var, char *value);
bool check_local_var(char *var, char *func_name);
bool unset_local(local_t *local, char *var);
char *get_local_value(local_t *local, char const *key);
local_t create_local(void);
#endif /* LOCAL_H */
