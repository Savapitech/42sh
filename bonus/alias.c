/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** parse_alias
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "alias.h"
#include "common.h"
#include "history.h"
#include "utils.h"

static
int skip_blank(const char *buffer, int i)
{
    for (; buffer[i] != 0 && isblank(buffer[i]); i++);
    return i;
}

static
int skip_to_next_token(char *buffer, int i)
{
    for (; buffer[i] != 0 && !is_a_token(buffer, i); i++);
    return i;
}

static
char *find_alias(his_variable_t *variable, alias_t *alias, char *buffer)
{
    char *cmd = malloc(sizeof(char) * (variable->size_variable + 1));
    char *new_cmd = nullptr;

    if (cmd == NULL)
        return nullptr;
    for (int i = variable->coord_variable; i !=
        variable->coord_variable + variable->size_variable; i++)
        cmd[i - variable->coord_variable] = buffer[i];
    cmd[variable->size_variable] = '\0';
    for (size_t i = 0; i != alias->size; i++){
        if (alias->alias_array[i] == NULL)
            return nullptr;
        if (strcmp(cmd, alias->alias_array[i]) == 0){
            new_cmd = cat_in_str(variable, buffer, alias->alias_to_replace[i]);
            buffer = new_cmd;
        }
    }
    free(cmd);
    return new_cmd;
}

static
char *get_alias(char *buffer, int i, alias_t *alias)
{
    int coord = i;
    int size = 0;
    his_variable_t variable = {0, 0, 0, nullptr, 0};

    for (; buffer[i] != 0 && !isblank(buffer[i])
        && !is_a_token(buffer, i); i++)
        size++;
    variable.coord_variable = coord;
    variable.size_variable = size;
    buffer = find_alias(&variable, alias, buffer);
    return buffer;
}

static
bool replace_alias(char **buffer, alias_t *alias)
{
    char *tmp_buff = *buffer;

    for (int i = skip_blank(tmp_buff, 0); tmp_buff[i] != 0; i++){
        i = skip_blank(tmp_buff, i);
        tmp_buff = get_alias(tmp_buff, i, alias);
        if (tmp_buff == NULL)
            return false;
        i = skip_to_next_token(tmp_buff, i);
    }
    *buffer = tmp_buff;
    return true;
}

int parse_alias(char **buffer, size_t *buffer_len, alias_t *alias)
{
    bool need_to_replace = true;

    while (need_to_replace)
        need_to_replace = replace_alias(buffer, alias);
    return RETURN_SUCCESS;
}

alias_t init_alias(void)
{
    alias_t alias;

    alias.size = 1;
    alias.alias_array = malloc(sizeof(char *) * alias.size);
    alias.alias_to_replace = malloc(sizeof(char *) * alias.size);
    if (!alias.alias_array || !alias.alias_to_replace)
        return alias;
    alias.alias_array[0] = nullptr;
    alias.alias_to_replace[0] = nullptr;
    alias.size = 0;
    return alias;
}
