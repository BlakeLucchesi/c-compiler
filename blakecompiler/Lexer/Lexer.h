//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#ifndef lexer_h
#define lexer_h
#import "Token.h"

typedef char (*take_next_t)(void *data);
typedef char (*peek_next_t)(void *data);

typedef struct _lexer_state {
    // Interface for reading data from a provider.
    take_next_t take_next;
    peek_next_t peek_next;
    void *data;

    // Public interface
    Token *start;

    // Internal state for the lexer.
    Token *head;
    char buffer[240];
    uint index;
    uint line_number;
    uint col_number;
} Lexer;


Lexer *CreateLexer(void);
void Lex(Lexer *lexer);
void LexerCleanup(Lexer *lexer);

void print_debug(Token *token);
const char *friendly_token_name(Token *token);

#endif
