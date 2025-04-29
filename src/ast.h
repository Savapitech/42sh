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

    #include "builtins_handler.h"

    #define DEFAULT_AST_CAP 128
    #define DEFAULT_N_LST_CAP 2
    #define DEFAULT_N_CMD_CAP 2
    #define DEFAULT_N_COND_CAP 2
    #define IF_PROMPT "if? "
    #define T_ALL 0xff

typedef enum : size_t {
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
    T_STAR = 1 << 18, // *
    T_NEWLINE = 1 << 19, // \n
    T_EOF = 1 << 20, // \0
    T_ARG = 1 << 21,
    T_INVALID = 1 << 22
} token_type_t;

typedef enum {
    N_LST,
    N_CMD,
    N_BIN,
    N_LOP,
    N_COND
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
        struct {
            ast_t *exp;
            size_t sz;
            size_t sz2;
            size_t cap;
            size_t cap2;
            ast_t **nodes;
            ast_t **nodes2;
        } cond;
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

// Main funcs
ast_t *parse_expression(ast_ctx_t *ctx);
token_t get_next_token(ast_ctx_t *ctx);

// Utils funcs
int visitor(char *buffer, exec_ctx_t *exec_ctx);
ast_t *create_node(ast_ctx_t *ctx);
bool ensure_node_cap(ast_t *node);
bool ensure_list_cap(ast_t *node);
bool ensure_cond_cap(ast_t *node);
bool ensure_cond_cap2(ast_t *node);
bool parser_eat(ast_ctx_t *ctx, token_type_t expected);
ast_t *parse_loop(ast_ctx_t *ctx);
void free_ast(ast_ctx_t *ctx);
void print_ast(ast_t *ast, ast_ctx_t *ctx, size_t depth);
void skip_semi(ast_ctx_t *ctx);

// Outside needed parser
ast_t *parse_cmd(ast_ctx_t *ctx);
ast_t *parse_semi(ast_ctx_t *ctx);
ast_t *parse_condition(ast_ctx_t *ctx);
ast_t *parse_and(ast_ctx_t *ctx, ast_t *l_node);
ast_t *parse_or(ast_ctx_t *ctx, ast_t *l_node);
ast_t *parse_if(ast_ctx_t *ctx);
#endif /* AST_H */
