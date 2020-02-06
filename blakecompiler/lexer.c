#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"


char take_next(FILE *input, char *buffer, uint *index);
char peek_next(FILE *input);

void emit_token(token **current, char *buffer, uint *index, token_name name);

bool is_number(char c);
bool is_letter(char c);
bool is_lowercase(char c);
bool is_uppercase(char c);
bool is_whitespace(char c);
bool is_identifier_character(char c);

token *lex(char *filename) {
    FILE *input = fopen(filename, "r");
    char c;
    
    char buffer[240];
    uint index = 0;
    
    token *head = (token *)malloc(sizeof(token));
    token *tail = head;
    while ((c = take_next(input, buffer, &index)) != EOF) {
        if (c == '/') {
            if (peek_next(input) == '/') {
                while (peek_next(input) != '\n') {
                    take_next(input, buffer, &index);
                }
                emit_token(&head, buffer, &index, COMMENT);
            }
            else {
                emit_token(&head, buffer, &index, OPERATOR);
            }
        }
        else if (c == ';') {
            emit_token(&head, buffer, &index, SEPARATOR);
        }
        else if (c == '{' || c == '}') {
            emit_token(&head, buffer, &index, SEPARATOR);
        }
        else if (c == '(' || c == ')') {
            emit_token(&head, buffer, &index, SEPARATOR);
        }
        else if (is_number(c)) { // ASCII integer values
            while (is_number(peek_next(input))) {
                take_next(input, buffer, &index);
            }
            emit_token(&head, buffer, &index, LITERAL);
        }
        else if (c == '+') {
            if (peek_next(input) == '+') {
                take_next(input, buffer, &index);
                emit_token(&head, buffer, &index, OPERATOR);
            }
            else {
                emit_token(&head, buffer, &index, OPERATOR);
            }
        }
        else if (c == '-') {
            emit_token(&head, buffer, &index, OPERATOR);
        }
        else if (c == '*') {
            emit_token(&head, buffer, &index, OPERATOR);
        }
        else if (c == '&') {
            emit_token(&head, buffer, &index, OPERATOR);
        }
        else if (c == '|') {
            emit_token(&head, buffer, &index, OPERATOR);
        }
        else if (is_identifier_character(c)) {
            while (is_identifier_character(peek_next(input))) {
                take_next(input, buffer, &index);
            }
            emit_token(&head, buffer, &index, IDENTIFIER);
        }
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
void emit_token(token **current, char *buffer, uint *index, token_name name) {
    token *tmp = (token *)calloc(1, sizeof(token));
    tmp->value = (char *)malloc((*index + 1)* sizeof(char));
    memcpy(tmp->value, buffer, *index);
    buffer[*index] = '\0';
    tmp->name = name;
    (*current)->next = tmp;
    *index = 0; // reset buffer index after token recorded.
    *current = tmp;
}


void print_debug(token *token) {
    printf("%15s | %s\n", friendly_token_name(token), token->value);
}

const char *friendly_token_name(token *token) {
    switch (token->name) {
        case OPERATOR:
            return "Operator";
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

void cleanup(token *head) {
//    while (
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
