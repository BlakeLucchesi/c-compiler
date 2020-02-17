//  Copyright © 2020 Blake Lucchesi. All rights reserved.

#include "generater.h"
#include "debug.h"

void process_function(AST_Function *function, FILE *output);
void process_statement(AST_Statement *statement, FILE *output);

void generate(AST_Program *program, FILE *output) {
    process_function(program->function, output);
}

void process_function(AST_Function *function, FILE *output) {
    fprintf(output, ".globl _%s\n", function->identifier->name);
    fprintf(output, "_%s:\n", function->identifier->name);
    process_statement(function->statement, output);
}

void process_statement(AST_Statement *statement, FILE *output) {
    fprintf(output, " movl $%s, %%eax\n", statement->expression->value);
    fprintf(output, " ret\n");
}
