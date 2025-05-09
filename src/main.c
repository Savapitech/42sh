/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "debug.h"
#include "shell.h"

const char OPT_FLAGS[] = "hc:";

static
void print_usages(FILE *file, char *bin)
{
    fprintf(file, "Usage: %s [-c command] [-dixv]"
        " [file]\n", bin);
}

static
bool parse_args(opt_t *opt, int ac, char **av)
{
    for (int o = getopt(ac, av, OPT_FLAGS); o != -1;
        o = getopt(ac, av, OPT_FLAGS)) {
        switch (o) {
            case 'h':
                exit((print_usages(stdout, av[0]), RETURN_SUCCESS));
            case 'c':
                opt->cmd = strdup(optarg);
                break;
            default:
                return print_usages(stderr, av[0]), false;
        }
    }
    if (optind < ac) {
        opt->script_file = av[optind];
        U_DEBUG("Script file [%s]\n", opt->script_file);
    } else {
        U_DEBUG_MSG("No script file provided.\n");
    }
    return true;
}

int main(int ac, char **av, char **env)
{
    opt_t opt = { 0, .script_file = nullptr, .cmd = nullptr };
    int result;

    U_DEBUG_MSG("Debug mode altivated.\n");
    if (!parse_args(&opt, ac, av))
        return RETURN_FAILURE;
    result = shell(&opt, env);
    free(opt.cmd);
    return result;
}
