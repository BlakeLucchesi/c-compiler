#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

// enum {
//     BRACE_OPEN,
//     BRACE_CLOSE
// }

char take_next(FILE *input, char *buffer);
char peek_next(FILE *input);

void emit_token(token **current, char value);

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
                emit_token(&head, c);
            }
        }
        if (c == '{') {
            emit_token(&head, c);
        }
        else if (c == '}') {
            emit_token(&head, c);
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
void emit_token(token **current, char value) {
    token *tmp = (token *)calloc(1, sizeof(token));
    tmp->value = value;
    (*current)->next = tmp;
    *current = tmp;
}

