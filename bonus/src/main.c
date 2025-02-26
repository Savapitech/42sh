/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include "debug.h"
#include "shell.h"

int main(int ac __attribute__((unused)), char **av __attribute__((unused)),
    char **env)
{
    U_DEBUG_MSG("Debug mode activated.");
    return shell(env);
}
