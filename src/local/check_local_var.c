/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** check_local_var
*/

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "env.h"
#include "exec.h"
#include "u_str.h"

bool check_local_var(char *var, char *func_name)
{
    if (!isalpha(var[0]))
        return (fprintf(stdout, "%s: Variable name must begin"
            " with a letter.\n", func_name), RETURN_FAILURE);
    if (!u_str_is_only_alnum(var))
        return (fprintf(stdout, "%s: Variable name must contain"
            " alphanumeric characters.\n", func_name), RETURN_FAILURE);
    return RETURN_SUCCESS;
}
