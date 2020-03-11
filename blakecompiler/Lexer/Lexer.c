//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Lexer.h"
#include "LexerMatchers.h"
#include "Keywords.h"
#include "Debug.h"

char take_next(Lexer *lexer);
char peek_next(Lexer *lexer);
void emit_token(Lexer *lexer, TokenClass klass, TokenName name);

Lexer *CreateLexer() {
    Lexer *lexer = calloc(1, sizeof(Lexer));
    lexer->head = (Token *)calloc(1, sizeof(Token));
    lexer->start = lexer->head;
    lexer->index = 0;
    lexer->line_number = 1;
    lexer->col_number = 1;
    return lexer;
}

void Lex(Lexer *lexer) {
    char c;
    TokenClass token_class = UNDEFINED_TOKEN;
    TokenName token_name = UNDEFINED_TOKEN_NAME;
    while ((c = take_next(lexer)) != EOF) {
        token_class = UNDEFINED_TOKEN;
        token_name = UNDEFINED_TOKEN_NAME;
        if (c == '\n') {
            lexer->line_number++;
            lexer->col_number = 1;
        }
        if (c == '/') {
            if (peek_next(lexer) == '/') {
                while (peek_next(lexer) != '\n') {
                    take_next(lexer);
                }
                token_class = COMMENT;
//                token_name = ;
            }
            else {
                token_class = OPERATOR;
                token_name = OP_DIVISOR;
            }
        }
        else if (c == ';') {
            token_class = SEPARATOR;
            token_name = SEP_SEMICOLON;
        }
        else if (c == '{') {
            token_class = SEPARATOR;
            token_name = SEP_BRACE_OPEN;
        }
        else if (c == '}') {
            token_class = SEPARATOR;
            token_name = SEP_BRACE_CLOSE;
        }
        else if (c == '(') {
            token_class = SEPARATOR;
            token_name = SEP_PAREN_OPEN;
        }
        else if (c == ')') {
            token_class = SEPARATOR;
            token_name = SEP_PAREN_CLOSE;
        }
        else if (c == '+') {
            if (peek_next(lexer) == '+') {
                take_next(lexer);
                token_class = OPERATOR;
//                 token_name = ;
            }
            else {
                token_class = OPERATOR;
                token_name = OP_ADDITION;
            }
        }
        else if (c == '-') {
            token_class = OPERATOR;
            token_name = OP_NEGATION;
        }
        else if (c == '*') {
            token_class = OPERATOR;
            token_name = OP_MULTIPLICATION;
        }
        else if (c == '&') {
            token_class = OPERATOR;
            token_name = OP_BITWISE_AND;
        }
        else if (c == '|') {
            token_class = OPERATOR;
            token_name = OP_BITWISE_OR;
        }
        else if (c == '!') {
            token_class = OPERATOR;
            token_name = OP_LOGICAL_NEGATION;
        }
        else if (c == '~') {
            token_class = OPERATOR;
            token_name = OP_BITWISE_COMPLEMENT;
        }
        else if (is_number(c)) {
            while (is_number(peek_next(lexer))) {
                take_next(lexer);
            }
            token_class = LITERAL;
        }
        else if (is_identifier_character(c)) {
            while (is_identifier_character(peek_next(lexer))) {
                take_next(lexer);
            }
            lexer->buffer[lexer->index] = '\0';
            token_class = is_keyword(lexer->buffer) || is_type(lexer->buffer) ? KEYWORD : IDENTIFIER;
            token_name = KEYWORD_RETURN;
        }
        if (token_class != UNDEFINED_TOKEN)
            emit_token(lexer, token_class, token_name);
        lexer->index = 0;
    }
    // Advance start pointer to first token.
    lexer->start = lexer->start->next;
}

char take_next(Lexer *lexer) {
    lexer->buffer[lexer->index] = lexer->take_next(lexer->data);
    lexer->col_number++;
    lexer->index++;
    if (lexer->index > 240)
        abort(); // buffer overflow
    return lexer->buffer[lexer->index - 1];
}
    
char peek_next(Lexer *lexer) {
    return lexer->peek_next(lexer->data);
}

void emit_token(Lexer *lexer, TokenClass klass, TokenName name) {
    Token *tmp = (Token *)calloc(1, sizeof(Token));
    tmp->line_number = lexer->line_number;
    tmp->col_number = lexer->col_number - lexer->index;
    tmp->value = (char *)malloc((lexer->index + 1)* sizeof(char));
    strncpy(tmp->value, lexer->buffer, lexer->index);
    tmp->value[lexer->index] = '\0';
    tmp->klass = klass;
    tmp->name = name;
    lexer->head->next = tmp;
    lexer->index = 0;
    lexer->head = tmp;
}

void print_debug(Token *token) {
    debug("Ln %2d, Col %2d | %12s | %s ", token->line_number, token->col_number, friendly_token_name(token), token->value);
}

const char *friendly_token_name(Token *token) {
    switch (token->klass) {
        case UNDEFINED_TOKEN:
            return "UNDEFINED";
        case OPERATOR:
            return "Operator";
            //    case BITWISE_COMPLEMENT:
            //        return "~";
            //    case NEGATION:
            //        return "Negation";
            //    case LOGICAL_NEGATION:
            //        return "NOT";
        case SEPARATOR:
            return "Separator";
        case IDENTIFIER:
            return "Identifier";
        case LITERAL:
            return "Literal";
        case COMMENT:
            return "Comment";
        case KEYWORD:
            return "Keyword";
    }
    return "UNKNOWN";
}

void LexerCleanup(Lexer *lexer) {
//    token *op = head;
//    while (op != NULL) {
//        free(op->value);
//        op = op->next;
//    }
}
