//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
// #include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "generater.h"
#include "debug.h"

int main(int argc, char* argv[]) {

    // todo allow debug logs via arg
    if (argc == 3) {
        LOGS_ENABLED = 1;
    }

    if (argc < 2) {
        debug("You must specify an input file.");
        return -1;
    }
    debug("Compiling %s", argv[1]);
   
    debug("Lexing...");
    Token *start = lex(argv[1]);
    Token *head = start;
    if (start->value == '\0') {
        debug("Error: Could not perform lexing.");
        return 10;
    }
    while (start != NULL) {
        print_debug(start);
        start = start->next;
    }
    debug("Parsing...");
    AST_Program *program = parse(&head);
    if (program == NULL) {
        debug("Error: Could not parse.");
        return 11;
    }
    debug("Generating Output");
    
    char *assembly_file_name = malloc(strlen(argv[1]));
    strcpy(assembly_file_name, argv[1]);
    assembly_file_name[strlen(assembly_file_name) - 1] = 's';
    FILE *assembly_file = fopen(assembly_file_name, "w");
    generate(program, assembly_file);
    fclose(assembly_file);
    
    debug("Assembling...");
    char *executable_file_name = malloc(strlen(argv[1]));
    strcpy(executable_file_name, argv[1]);
    executable_file_name[strlen(executable_file_name) - 2] = '\0';
    
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        debug("child executing $ gcc %s -o %s", assembly_file_name, executable_file_name);
        int r = execl("/usr/bin/gcc", "gcc", assembly_file_name, "-o", executable_file_name, (char *)0);
        printf("GCC Failed: return:%d. errno:%d\n", r, errno);
        abort();
    }
    else {
        debug("Parent %d waiting for %d", getpid(), pid);
        waitpid(pid, &status, 0);
        debug("Child %d exited %d", pid, status);
        debug("Complete!");
    }
    
//    remove(assembly_file_name);
    free(assembly_file_name);
    free(executable_file_name);
    cleanup(head);
    return 0;
}


