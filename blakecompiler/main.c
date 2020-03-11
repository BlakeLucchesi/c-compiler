//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
// #include <stdlib.h>

#include "FileLexer.h"
#include "Parser.h"
#include "Generater.h"
#include "Debug.h"

int main(int argc, char* argv[]) {

    // todo allow debug logs via arg
    if (argc == 3) {
        LOGS_ENABLED = 1;
    }

    if (argc < 2) {
        debug("You must specify an input file.");
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        debug("Could not open input file %s.", argv[1]);
        return 1;
    }
    debug("Starting to compile %s", argv[1]);
   
    debug("\n--Lexing--");
    
    Lexer *lexer = CreateFileLexer(input);
    Lex(lexer);
    Token *start = lexer->start;
    fclose(input);
    if (start->value == '\0') {
        debug("Error: Could not perform lexing.");
        return 10;
    }
    while (start != NULL) {
        print_debug(start);
        start = start->next;
    }
    debug("\n--Parsing--");
    ASTProgram *program = ASTParse(lexer->start);
    ASTError *error = ASTGetErrors();
    
    if (error != NULL) {
        debug("Error: Could not parse.");
        while (error != NULL) {
            ASTPrintError(error);
            error = error->next;
        }
        return 11;
    }

    debug("\n--Generating Output--");
    char *assembly_file_name = malloc(strlen(argv[1]));
    strcpy(assembly_file_name, argv[1]);
    assembly_file_name[strlen(assembly_file_name) - 1] = 's';
    FILE *assembly_file = fopen(assembly_file_name, "w");
    generate(program, assembly_file);
    fclose(assembly_file);
    
    debug("\n--Assembling--");
    char *executable_file_name = malloc(strlen(argv[1]));
    strcpy(executable_file_name, argv[1]);
    executable_file_name[strlen(executable_file_name) - 2] = '\0';
    
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        int r = execl("/usr/bin/gcc", "gcc", assembly_file_name, "-o", executable_file_name, (char *)0);
        printf("GCC Failed: return:%d. errno:%d\n", r, errno);
        abort();
    }
    else {
        waitpid(pid, &status, 0);
        debug("\n--Complete!--");
    }
    
    remove(assembly_file_name);
    free(assembly_file_name);
    free(executable_file_name);
    LexerCleanup(lexer);
    return 0;
}


