//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef lexer_h
#define lexer_h

typedef enum _token_class {
    UNDEFINED_TOKEN,
    OPERATOR,
    SEPARATOR,
    IDENTIFIER,
    KEYWORD,
    LITERAL,
    COMMENT,
} TokenClass;

typedef enum _operator_type {
    OP_NEGATION,
    OP_LOGICAL_NEGATION,
    OP_BITWISE_COMPLEMENT,
    OP_ADDITION,
    OP_MULTIPLICATION,
    OP_DIVISOR,
} OpType;

typedef enum _separator_type {
    SEP_SEMICOLON,
    SEP_PAREN_OPEN,
    SEP_PAREN_CLOSE,
    SEP_BRACE_OPEN,
    SEP_BRACE_CLOSE,
} SepType;

typedef enum _keyword_type {
    KEYWORD_RETURN,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_INT,
} KeywordType;

typedef struct _token {
    char *value;
    TokenClass klass;
    union {
        OpType op;
        SepType sep;
        KeywordType key;
    };
    struct _token *next;
} token;


token *lex(char *input);

void print_debug(token *token);
const char *friendly_token_name(token *token);

// walks through lexemes and deallocates managed memory.
void cleanup(token *head);

#endif
