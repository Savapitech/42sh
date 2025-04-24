/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#ifndef BUILTINS_H
    #define BUILTINS_H
    #include "exec.h"

typedef struct {
    char const *name;
    int (*ptr)(ef_t *ef, char **args);
} builtins_funcs_t;

extern const builtins_funcs_t BUILTINS[];
extern const size_t BUILTINS_SZ;

int builtins_exit(ef_t *ef, char **args);
int builtins_env(ef_t *ef, char **args);
int builtins_setenv(ef_t *ef, char **args);
int builtins_unsetenv(ef_t *ef, char **args);
int builtins_cd(ef_t *ef, char **args);
int builtins_builtins(ef_t *ef, char **args);
int builtins_funny_double_dot(ef_t *ef, char **args);
int builtins_history(ef_t *ef, char **args);
int builtins_alias(ef_t *ef, char **args);
int builtins_display_alias(alias_t *alias);
int builtins_repeat(ef_t *ef, char **args);
int builtins_yes(ef_t *ef, char **args);
int builtins_foreach(ef_t *ef, char **args);
int builtins_set(ef_t *ef, char **args);
int builtins_unset(ef_t *ef, char **args);
int builtins_where(ef_t *ef, char **args);
int builtins_which(ef_t *ef, char **args);
#endif /* BUILTIND_H */
