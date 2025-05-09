/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#ifndef BUILTINS_H
    #define BUILTINS_H
    #include "exec.h"
    #include "u_str.h"

typedef struct {
    char **cmds;
    size_t size;
    size_t cap;
} cmd_block_t;

typedef struct {
    buff_t *buff;
    bool in_else;
    ef_t *ef;
} if_ctx_t;

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
int builtins_while(ef_t *ef, char **args);
int builtins_set(ef_t *ef, char **args);
int builtins_unset(ef_t *ef, char **args);
int builtins_where(ef_t *ef, char **args);
int builtins_which(ef_t *ef, char **args);
int builtins_break(ef_t *ef, char **args);
int builtins_expr(ef_t *ef, char **args);
int builtins_if(ef_t *ef, char **args);
int builtins_echo(ef_t *ef, char **args);
#endif /* BUILTIND_H */
