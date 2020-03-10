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

typedef struct _lex_state {
    FILE *input;
    char buffer[240];
    Token *head;
    Token *tail;
    uint index;
    uint line_number;
    uint col_number;
} Lexer;

//extern LexerState _lex_state;

Lexer *MakeLexer(void);
Token *Lex(Lexer *state);

void print_debug(Token *token);
const char *friendly_token_name(Token *token);

// walks through lexemes and deallocates managed memory.
void cleanup(Token *head);

#endif
