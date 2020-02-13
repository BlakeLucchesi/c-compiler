//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdio.h>
// #include <stdlib.h>

#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("You must specify an input file.\n");
        return -1;
    }
    printf("Compiling %s\n", argv[1]);
   
    printf("Lexing...\n");
    token *start = lex(argv[1]);
    token *head = start;
    if (start->value == '\0') {
        printf("Error: Could not perform lexing.\n");
        return 10;
    }
    while (start != NULL) {
        print_debug(start);
        start = start->next;
    }
    printf("Parsing...\n");
    parse(&head);
    
    printf("Complete!\n");
    cleanup(head);
    return 0;
}

