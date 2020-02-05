#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"


char take_next(FILE *input, char *buffer);
char peek_next(FILE *input);

void emit_token(token **current, char value, token_name name);

token *lex(char *filename) {
    FILE *input = fopen(filename, "r");
    char c;
    
    char buffer[240];
    uint index = 0;
    
    token *head = (token *)malloc(sizeof(token));
    token *tail = head;
    while ((c = take_next(input, &buffer[index])) != EOF) {
        if (c == '/') {
            if (peek_next(input) == '/') {
                while (take_next(input, &buffer[index]) != '\n') {
                    continue;
                }
                index = 0;
            }
            else {
                emit_token(&head, c, OPERATOR);
            }
        }
        if ((int)c >= 48 && (int)c <= 57) { // ASCII integer values
            emit_token(&head, c, VALUE);
        }
        if (c == '+') {
            emit_token(&head, c, OPERATOR);
        }
        if (c == '{') {
            emit_token(&head, c, BRACE);
        }
        else if (c == '}') {
            emit_token(&head, c, BRACE);
        }
    }
    fclose(input);
    return tail->next;
}

char take_next(FILE *input, char *buffer) {
    *buffer = fgetc(input);
    return *buffer;
}
    
char peek_next(FILE *input) {
    char next = fgetc(input);
    fseek(input, -1, SEEK_CUR);
    return next;
}

// TODO: RESET BUFFER INDEX when passing buffer instead of value.
void emit_token(token **current, char value, token_name name) {
    token *tmp = (token *)calloc(1, sizeof(token));
    tmp->value = value;
    tmp->name = name;
    (*current)->next = tmp;
    *current = tmp;
}


void print_debug(token *token) {
    printf("%10s | %c\n", friendly_token_name(token), token->value);
}

const char *friendly_token_name(token *token) {
    switch (token->name) {
        case OPERATOR:
            return "operator";
        case BRACE:
            return "brace";
        case VALUE:
            return "value";
    }
    return "UNKNOWN";
}
