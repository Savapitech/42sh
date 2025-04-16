/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef AST_H
    #define AST_H
    #include <stddef.h>
    #include <stdint.h>

    #include "shell.h"
    #include "env.h"

    #define DEFAULT_AST_CAP 128
    #define T_ALL 0xff

typedef enum {
    T_SEMICOLON = 1 << 0, // ;
    T_LEFT_QUOTE = 1 << 1, // "
    T_RIGHT_QUOTE = 1 << 2, // "
    T_AND = 1 << 3, // &&
    T_OR = 1 << 4, // ||
    T_PIPE = 1 << 5, // |
    T_BACKTICK = 1 << 6, // `
    T_LEFT_PARENT = 1 << 7, // (
    T_RIGHT_PARENT = 1 << 8, // )
    T_PREV_CMD = 1 << 9, // !!
    T_VAR = 1 << 10, // $
    T_REDIRECT = 1 << 11, // >
    T_APPEND = 1 << 12, // >>
    T_HEREDOC = 1 << 13, // <
    T_IN_REDIRECT = 1 << 14, // <
    T_AT = 1 << 15, // <
    T_WHILE = 1 << 16, // while
    T_FOREACH = 1 << 17, // foreach
    T_EOF = 1 << 18, // \0
    T_ARG = 1 << 19,
    T_INVALID = 1 << 20
} token_type_t;

typedef enum {
    N_LST,
    N_CMD,
    N_BIN,
    N_LOP
} node_type_t;

typedef struct {
    token_type_t type;
    char *str;
    size_t sz;
} token_t;

typedef struct {
    token_type_t type;
    char const *str;
    uint8_t sz;
    char const *name;
} tokens_list_t;

typedef struct ast_s ast_t;

typedef struct ast_s {
    node_type_t type;
    union {
        struct {
            ast_t *left;
            ast_t *right;
        } binary;
        struct {
            size_t sz;
            size_t cap;
            token_t *tokens;
        } vector;
        struct {
            size_t sz;
            size_t cap;
            ast_t **nodes;
        } list;
        struct {
            size_t sz;
            size_t cap;
            char **buffers;
            ast_t *condition;
        } loop;
    };
    token_t tok;
} ast_t;

typedef struct {
    size_t i;
    size_t sz;
    char *str;
    size_t cap;
    ast_t *ast;
    token_t act_tok;
    ast_t *first_node;
} ast_ctx_t;


extern const tokens_list_t TOKENS_LIST[];

ast_t *parse_expression(ast_ctx_t *ctx);
void print_ast(ast_ctx_t *ctx, ast_t *ast, size_t depth);
token_t get_next_token(ast_ctx_t *ctx);
int visitor(char *buffer, env_t *env, history_t *history);
ast_t *create_node(ast_ctx_t *ctx);
bool ensure_node_cap(ast_t *node);
bool ensure_list_cap(ast_t *node);
bool parser_eat(ast_ctx_t *ctx, token_type_t expected);
ast_t *parse_loop(ast_ctx_t *ctx);
void free_ast(ast_ctx_t *ctx);
#endif /* AST_H */
