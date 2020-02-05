#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

// enum {
//     BRACE_OPEN,
//     BRACE_CLOSE
// }

token *lex(char *filename) {
    FILE *input = fopen(filename, "r");
    char c;
    
    token *head = (token *)malloc(sizeof(token));
    token *tail = head;
//    char current = *input;
    while ((c = fgetc(input)) != EOF) {
        if (c == '{') {
            head->value = c;
            token *tmp = (token *)malloc(sizeof(token));
            head->next = tmp;
            head = tmp;
        }
        if (c == '}') {
            head->value = c;
            token *tmp = (token *)malloc(sizeof(token));
            head->next = tmp;
            head = tmp;
        }
    }
    fclose(input);
    return tail;
}

