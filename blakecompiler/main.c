#include <stdio.h>
// #include <stdlib.h>

#include "lexer.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("you must specify an input file.\n");
        return -1;
    }
    printf("compiling %s\n", argv[1]);
   
    token *start = lex(argv[1]);

    if (start->value == '\0') {
        printf("could not perform lexing.\n");
        return 10;
    }
    while (start != NULL) {
        printf("%c\n", start->value);
        start = start->next;
    }
    printf("FINISHED\n");
    return 0;
}

