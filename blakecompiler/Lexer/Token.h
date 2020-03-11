//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef Token_h
#define Token_h

#include <stdio.h>

typedef enum _token_class {
    UNDEFINED_TOKEN,
    OPERATOR,
    SEPARATOR,
    IDENTIFIER,
    KEYWORD,
    LITERAL,
    COMMENT,
} TokenClass;

typedef enum _token_name {
    UNDEFINED_TOKEN_NAME,
    OP_NEGATION,
    OP_LOGICAL_NEGATION,
    OP_BITWISE_COMPLEMENT,
    OP_BITWISE_AND,
    OP_BITWISE_OR,
    OP_ADDITION,
    OP_MULTIPLICATION,
    OP_DIVISOR,
    SEP_SEMICOLON,
    SEP_PAREN_OPEN,
    SEP_PAREN_CLOSE,
    SEP_BRACE_OPEN,
    SEP_BRACE_CLOSE,
    KEYWORD_RETURN,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_INT,
} TokenName;

typedef struct _token {
    char *value;
    uint32_t line_number;
    uint32_t col_number;
    TokenClass klass;
    TokenName name;
    struct _token *next;
} Token;

#endif /* Token_h */
