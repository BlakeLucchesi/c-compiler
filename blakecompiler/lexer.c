#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

// enum {
//     BRACE_OPEN,
//     BRACE_CLOSE
// }

void emit_token(token **current, char value);

token *lex(char *filename) {
    FILE *input = fopen(filename, "r");
    char c;
    
    token *head = (token *)malloc(sizeof(token));
    token *tail = head;
//    char current = *input;
    while ((c = fgetc(input)) != EOF) {
        if (c == '{') {
            emit_token(&head, c);
        }
        if (c == '}') {
            emit_token(&head, c);
        }
    }
    fclose(input);
    return tail->next;
}

void emit_token(token **current, char value) {
    token *tmp = (token *)calloc(1, sizeof(token));
    tmp->value = value;
    (*current)->next = tmp;
    *current = tmp;
}

