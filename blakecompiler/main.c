//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "generater.h"
#include "debug.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        debug("You must specify an input file.\n");
        return -1;
    }
    debug("Compiling %s\n", argv[1]);
   
    debug("Lexing...\n");
    token *start = lex(argv[1]);
    token *head = start;
    if (start->value == '\0') {
        debug("Error: Could not perform lexing.\n");
        return 10;
    }
    while (start != NULL) {
        print_debug(start);
        start = start->next;
    }
    debug("Parsing...\n");
    AST_Program *program = parse(&head);
    if (program == NULL) {
        debug("Error: Could not parse.\n");
        return 11;
    }
    debug("Generating Output\n");
    
    char *assembly_file_name = malloc(strlen(argv[1]));
    strcpy(assembly_file_name, argv[1]);
    assembly_file_name[strlen(assembly_file_name) - 1] = 'a';
    FILE *assembly_file = fopen(assembly_file_name, "w");
    generate(program, assembly_file);
    fclose(assembly_file);
    
//    debug("Assembling...");
//    const char *executable_file_name = "";
//    exec("gcc %s -o %s", assembly_file_name, executable_file_name);
    
    debug("Complete!\n");
    cleanup(head);
    return 0;
}


