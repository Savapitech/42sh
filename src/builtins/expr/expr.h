/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef EXPR_H
    #define EXPR_H

    #include <stdbool.h>
    #include <stddef.h>

    #define FIRST_TOKEN_BATCH_SIZE 64
    #define IS_CHAR(s, c) ((s)[0] == (c) && (s)[1] == '\0')

    #define COUNT_OF(arr) (sizeof (arr) / (sizeof *(arr)))

typedef unsigned char uchar_t;

typedef enum {
    E_VAL_INT = 'd',
    E_VAL_STR = 's',
    E_VAL_ERR = 'e',
} expr_val_type_t;

typedef struct {
    char *p;
    union {
        long val;
        char const *str;
    };
    char type;
} expr_val_t;

typedef struct {
    char name[4];
    unsigned int prec;
    expr_val_t (*apply)(expr_val_t *, expr_val_t *);
} expr_op_precedence_t;

typedef struct {
    char **args;
} expr_state_t;

expr_val_t expr_run(expr_state_t *state, uchar_t depth, int prec);

extern const expr_op_precedence_t OPERATOR_PRECEDENCE[];
extern const size_t OPERATOR_PRECEDENCE_COUNT;

#endif
