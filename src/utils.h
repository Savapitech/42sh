/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/


#ifndef UTILS_H
    #define UTILS_H
    #include "history.h"
    #include "builtins_handler.h"
    #include "u_str.h"

char *strn_to_ndup(int start, int size, char *str);
bool is_a_token(char *str, int index_str);
char *cat_in_str(his_variable_t *his_variable, char *str, char *cpy);
int len_array(char **array);
char *insert_str(const char *base, const char *insert, size_t pos);
void free_everything(exec_ctx_t *exec_ctx);
void free_args(char **args);
bool ensure_buff_av_capacity(buff_t *buff, size_t requested);
#endif /* UTILS_H */
