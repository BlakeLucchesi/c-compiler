//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"
#include "keywords.h"
#include "debug.h"

char take_next(FILE *input, char *buffer, uint *index);
char peek_next(FILE *input);

void emit_token(Token **current, char *buffer, uint *index, TokenClass name);

bool is_number(char c);
bool is_letter(char c);
bool is_lowercase(char c);
bool is_uppercase(char c);
bool is_whitespace(char c);
bool is_identifier_character(char c);

Token *lex(char *filename) {
    FILE *input = fopen(filename, "r");
    char c;
    
    char buffer[240];
    uint index = 0;
    
    Token *head = (Token *)calloc(1, sizeof(Token));
    Token *tail = head;
    TokenClass token_class = UNDEFINED_TOKEN;
//    TokenName token_name = OPERATOR_DIVISOR;
    while ((c = take_next(input, buffer, &index)) != EOF) {
        token_class = UNDEFINED_TOKEN;
        if (c == '/') {
            if (peek_next(input) == '/') {
                while (peek_next(input) != '\n') {
                    take_next(input, buffer, &index);
                }
                token_class = COMMENT;
            }
            else {
                token_class = OPERATOR;
            }
        }
        else if (c == ';') {
            token_class = SEPARATOR;
//            token_name = SEPARATOR_SEMICOLON;
        }
        else if (c == '{') {
            token_class = SEPARATOR;
//            token_name = SEPARATOR_BRACE_OPEN;
        }
        else if (c == '}') {
            token_class = SEPARATOR;
//            token_name = SEPARATOR_BRACE_CLOSE;
        }
        else if (c == '(') {
            token_class = SEPARATOR;
//            token_name = SEPARATOR_PAREN_OPEN;
        }
        else if (c == ')') {
            token_class = SEPARATOR;
//            token_name = SEPARATOR_PAREN_CLOSE;
        }
        else if (c == '+') {
            if (peek_next(input) == '+') {
                take_next(input, buffer, &index);
                token_class = OPERATOR;
            }
            else {
                token_class = OPERATOR;
            }
        }
        else if (c == '-') {
            token_class = OPERATOR;
//            token_name = OPERATOR_NEGATION;
        }
        else if (c == '*') {
            token_class = OPERATOR;
//            token_name = OPERATOR_MULTIPLICATION;
        }
        else if (c == '&') {
            token_class = OPERATOR;
//            token_name = OPERATOR_LOGICAL_NEGATION
        }
        else if (c == '|') {
            token_class = OPERATOR;
        }
        else if (c == '!') {
            token_class = OPERATOR;
//            token_name = OPERATOR_LOGICAL_NEGATION;
        }
        else if (c == '~') {
            token_class = OPERATOR;
//            token_name = OPERATOR_BITWISE_COMPLEMENT;
        }
        else if (is_number(c)) { // ASCII integer values
            while (is_number(peek_next(input))) {
                take_next(input, buffer, &index);
            }
            token_class = LITERAL;
        }
        else if (is_identifier_character(c)) {
            while (is_identifier_character(peek_next(input))) {
                take_next(input, buffer, &index);
            }
            char *value = malloc(sizeof(char) * (index + 1));
            strncpy(value, buffer, index); // TODO: do we need to copy?
            token_class = is_keyword(value) || is_type(value) ? KEYWORD : IDENTIFIER;
            free(value);
        }
        if (token_class != UNDEFINED_TOKEN)
            emit_token(&head, buffer, &index, token_class);
        index = 0;
    }
    fclose(input);
    return tail->next;
}

char take_next(FILE *input, char buffer[], uint *index) {
    buffer[*index] = fgetc(input);
    (*index)++;
    if (*index > 240)
        abort(); // buffer overflow
    return buffer[*index - 1];
}
    
char peek_next(FILE *input) {
    char next = fgetc(input);
    fseek(input, -1, SEEK_CUR);
    return next;
}

// TODO: RESET BUFFER INDEX when passing buffer instead of value.
void emit_token(Token **current, char *buffer, uint *index, TokenClass name) {
    Token *tmp = (Token *)calloc(1, sizeof(Token));
    tmp->value = (char *)malloc((*index + 1)* sizeof(char));
    strncpy(tmp->value, buffer, *index);
    tmp->value[*index] = '\0';
    tmp->klass = name;
    (*current)->next = tmp;
    *index = 0; // reset buffer index after token recorded.
    *current = tmp;
}


void print_debug(Token *token) {
    debug("%15s | %s", friendly_token_name(token), token->value);
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

void cleanup(Token *head) {
//    token *op = head;
//    while (op != NULL) {
//        free(op->value);
//        op = op->next;
//    }
}

bool is_number(char c) {
    return (int)c >= 48 && (int)c <= 57;
}

bool is_identifier_character(char c) {
    return is_letter(c) || is_number(c) || c == '_';
}

bool is_letter(char c) {
    return (int)c >= (int)'A' && (int)c <= (int)'z';
}

bool is_uppercase(char c) {
    return (int)c >= (int)'A' && (int)c <= (int)'Z';
}

bool is_lowercase(char c) {
    return (int)c >= (int)'a' && (int)c <= (int)'z';
}

bool is_whitespace(char c) {
    return c == '\n' || c == ' ' || c ==  '\r' || c == '\t';
}
